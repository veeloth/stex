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
  int integer;
  unsigned char uchars[sizeof(int)];
  char chars[sizeof(int)];
  };


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
  strcpy(buf_name, argv[1]);
  sscanf(argv[2], "%zu", &tmp);
  move = constrained_move;
  if (!(main_buffer =
    shtex_create(buf_name, tmp?tmp:65536) ))
    exit((perror("couldn't create buffer"), 1));
  }

void search()
  {
  char* snip;//buffer containing whatever user wants to search for
  char snipname[256];

  sprintf(snipname, "%s.snip", buf_name);
  snip = shtex_create(snipname, 256)->data;

  sprintf(command, "%s %s", self, snipname);
  system(command);

  go(where(snip));
  shm_unlink(snipname);
  }

void ktrl(unsigned char length, unsigned char input_arr[static 4])
  {//input processing function
  union input input = {.integer = 0};
  msg[0] = 0;

  if (length==255) exit(0);//l == 255 iff input == ctrl+c 
  for (char i = 0; i < length; ++i)
    input.uchars[i] = input_arr[i];
  if (iscntrl(input.chars[0])) switch (input.integer)
      {
      case left: move(-1); break;
      case right: move(1); break;
      case backspace: delete(1); break;
      case 8: mb_delete(1); break;
      case supr: delete(-1); break;
      case 5069595: mb_delete(-1); break;
      case 15: move = constrained_move; break;
      case 16: move = mb_move; break;
      case ctrl_d:
        sprintf(msg, "%d", mblen(buf+cur, 255));
        break;
      case ctrl_f: search(); break;
      case ctrl_l: buf[cur] = 0; break;
      case 9: buf[cur]++; break;
      case 24: buf[cur]--; break;
      case ctrl_w: delete(cur - prev_word()); break;
      case ctrl_e: delete(cur - next_word()); break;
      case escape: insert("\e", 1); break;
      case enter: insert("\n", 1); break;
      default://this code runs when the keysym has no set behavior
        for (char i = 0; i < length; ++i) sprintf
          (msg+strlen(msg), "%02x", input.uchars[i]);
        sprintf(msg+strlen(msg), " %d %d",
                length, input.integer);
      }
  else insert(input.chars, length);
  }
