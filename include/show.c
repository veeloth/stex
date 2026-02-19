#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <termios.h>
#include <sys/mman.h>

#include "shtex.h"
#include "term.c"

#define true 1
#define false 0
#define BAR "%s[%zu]: %02x  size: %zu "
#define INT(x) (int)((x)%INT_MAX)

#define cur main_buffer->cursor
#define cap main_buffer->size
#define msg main_buffer->snip
#define buf main_buffer->data

struct tex* main_buffer;
struct winsize ws;
struct termios prevstate;

size_t sposx = 0;
size_t sposy = 0;
char done = false;
char bufname[256];
char self[258];
char bar[256] = BAR;


//func declarations
void drawbar()
  {
  printf("\e[%d;1H", ws.ws_col);//move to lowest row
  printf(bar, bufname, cur, buf[cur], cap);//print bar and go back
  printf("msg: %s ", msg);
  printf("x: %zu ; y: %zu", sposx, sposy);
  }


//main flow declarations
void draw()
  {//UI / printing function
  printf("\e[40m\e[39m \e[2J");//set color to normal and clear screen
  printf("\e[1;1H");//go back to beginning of screen
  printf("%.*s", INT(cur), buf);
  getcur(&sposx, &sposy);
  printf("%.*s", INT(cap-cur), buf+cur);
  drawbar();
  printf("\e[%zu;%zuH", sposx, sposy);//move cursor to its position on screen
  }


void drop()
  {//terminate function
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &prevstate);//precisely what it looks like, sets terminal to previous state / mode
  printf("\e[40m\e[39m\e[2J\e[1;1H");//clears screen... and sets colors to normal
  }


void init(char** argv)
  {
  atexit(drop);
  prepare_terminal_normal_sa("show", &ws, &prevstate);

  strcpy(self, argv[0]);
  strcpy(bufname, argv[1]);

  if (!(main_buffer = shtex_create(bufname, 256*256)))
    exit(1);
  }


//
int main(int argc, char* argv[argc+1])
  {
  if (argc!=2) return printf("usage: %s <buf name>", argv[0]), 1;
  init(argv);
  while (!done) draw(), usleep(16700);
  drop();
  return 0;
  }
