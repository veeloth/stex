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
union input input;//note: c stands for character, cin is "character input"
struct winsize ws;
struct termios prevstate;


//main flow declarations
void hex_ktrl_exit()
  {//terminate function
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &prevstate);//precisely what it looks like, sets terminal to previous state / mode
  }


void hex_ktrl_init(char** argv)
  {
  atexit(hex_ktrl_exit);
  prepare_terminal("ktrl", &ws, &prevstate);
  strcpy(self, argv[0]);
  strcpy(bufname, argv[1]);
  move = constrained_move;
  if (!(main_buffer = shtex_create(bufname, 256*256)))
    exit((perror("couldn't create buffer"), 1));
  }

void hex_ktrl(unsigned char len, unsigned char input_arr[static 4])
  {//input processing function
  input.integer = 0;
  if (len==255) exit(0);//l == 255 iff input == ctrl+c 
  //get input, can't assign *(int*)input_arr to input.integer directly bcs not all bytes are updated
  for (char i = 0; i < len; ++i)
    input.uchars[i] = input_arr[i];
  msg[0] = 0;
  if (iscntrl(input.chars[0]))
    {
    switch (input.integer)
      {
      case left:
        move(-1);
        break;
      case right:
        move(1);
        break;
      case backspace:
        delete(1);
        break;
      case ctrl_f:
        search();
        break;
      case enter:
        insert("\n", 1);
        break;
      default://this code runs when the keysym has no set behavior
        sprintf(msg, "\e[30m\e[41m0x");
        for (char i = 0; i < len; ++i)
          sprintf(msg+strlen(msg), "%02x", input.uchars[i]);
        sprintf(msg+strlen(msg), " %d %d\e[0m", len, input.integer);
      }
    }
  else insert(input.chars, len);
  }
