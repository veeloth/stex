#include <locale.h>
#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>

#include "keys.h"
#include "moda.c"
#include "term.c"

#define INT(x) (int)((x)%INT_MAX)

union input
  {
  unsigned char uchars[sizeof(int)];
  char chars[sizeof(int)];
  int integer;
  };
char stex_name[256];
size_t arg_size;

void (*move)(ptrdiff_t) = constrained_move;
void def_enter() { insert("\n", 1); }
void (*on_enter)() = def_enter;
void search() { go(where(arg)); }

void to_buf()
  {//starts writing to buf
  main_cursor = &main_buffer->cursor;
  main_size = &main_buffer->size;
  main_data = main_buffer->data;
  on_enter = def_enter;
  }

/*
 * IDEA: instead of current implementation of arg
 * you could have the program recursively call itself
 * but have the callee's buf be the current arg, and 
 * the program's exit key be enter, to do this I'll
 * have to implement a "recursive" mode for ktrl,
 * which would limit the different ways of impllementation
 */

//by default, action is nothing
void (*action)() = to_buf;
void use_arg()
  {//performs action and resets arg
  to_buf();
  action();
  kur = arg[0] = 0;
  }

void get_arg()
  {//starts writing to arg and performs action on enter
  main_data = main_buffer->sarg;
  main_cursor = &main_buffer->kursor;
  main_size = &arg_size;
  on_enter = use_arg;
  }

void hexsprint(char* dst, unsigned char* src, unsigned char length)
  {
  for (unsigned char i = 0; i < length; i++)
    sprintf(dst+strlen(dst), "%02hhx", src[i]);
  }

//main flow declarations
void ktrl_exit()
  {//terminate function
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &prevstate);//precisely what it looks like, sets terminal to previous state / mode
  }

void ktrl_init(char** argv)
  {
  setlocale(LC_CTYPE, "");
  size_t tmp;
  atexit(ktrl_exit);
  prepare_terminal("ktrl", &ws, &prevstate);

  strcpy(self, argv[0]);
  strcpy(stex_name, argv[1]);
  arg_size = sizeof(arg);

  sscanf(argv[2], "%zu", &tmp);
  if (stex_init(stex_name, tmp)) exit(1);
  }

void ktrl(unsigned char length, unsigned char input_arr[static 4])
  {//input processing function
  union input input = {.integer = 0};
  msg[0] = 0;

  for (char i = 0; i < length; ++i)
    input.uchars[i] = input_arr[i];
  if (iscntrl(input.chars[0])) switch (input.integer)
      {
      case left: move(-1); break;
      case right: move(1); break;
      case backspace: delete(-1); break;
      case 8: mb_delete(-1); break;
      case supr: delete(1); break;
      case 5069595: mb_delete(1); break;
      case ctrl_c: exit(0);
      case ctrl_e: delete(mbc(cur, next_word() - cur)); break;
      case ctrl_w: delete(-mbc(cur, prev_word() - cur)); break;
      case ctrl_f: action = search, get_arg(); break;
      case ctrl_i: buf[cur]++; break;
      case ctrl_o:
        move = move==mb_move?constrained_move:mb_move;
        printf(move==mb_move?"mb\n":"~mb\n");
        break;
      case escape: insert("\e", 1); break;
      case enter: on_enter(); break;
      default://this code runs when the keysym has no set behavior
        for (char i = 0; i < length; ++i) sprintf
          (msg+strlen(msg), "%02x", input.uchars[i]);
        sprintf(msg+strlen(msg), " %d %d",
                length, input.integer);
      }
  else insert(input.chars, length);
  }
