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

void (*move)(ptrdiff_t) = mb_move;
void def_enter() { insert("\n", 1); }
void (*on_enter)() = def_enter;
void search() { go(where(arg)); }

void posgo()
  {
  size_t pos = 0;
  sscanf(arg, "%zu", &pos);
  go(pos);
  }

void to_buf()
  {//starts writing to buf
  current_cursor = &main_typr->cursor;
  current_size = &main_typr->size;
  current_str = main_typr->data;
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
  current_cursor = &main_typr->kursor;
  current_size = &arg_size;
  current_str = main_typr->sarg;
  on_enter = use_arg;
  }

//main flow declarations
void ktrl_exit()
  {//terminate function
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &prevstate);//precisely what it looks like, sets terminal to previous state / mode
  }

void ktrl_init(char** argv)
  {
  mb_init();
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
      case up: line_up(); break;
      case down: line_down(); break;
      case right: move(1); break;
      case left: move(-1); break;
      case backspace: mb_delete(-1); break;
      case supr: mb_delete(1); break;
      case ctrl_a: return;
      case ctrl_c: exit(0);
      case ctrl_e:
        delete(mbc(cur, next_word() - cur)); break;
      case ctrl_w:
        delete(-mbc(cur, prev_word() - cur)); break;
      case ctrl_f: action = search, get_arg(); break;
      case ctrl_g: action = posgo, get_arg(); break;
      case ctrl_i: buf[cur]++; break;
      case ctrl_o:
        move = move==mb_move?constrained_move:mb_move;
        printf(move==mb_move?"mb\n":"~mb\n");
        break;
      case escape: insert("\e", 1); break;
      case enter: on_enter(); break;
      default:
        sprintf(msg+strlen(msg),"0x%x", input.integer);
        sprintf(msg+strlen(msg), " %d", input.integer);
        sprintf(msg+strlen(msg), " %d", length);
      }
  else insert(input.chars, length);
  }
