#include <limits.h>
#include <string.h>

#include "texbuf.c"
#include "term.c"

#define INT(x) (int)((x)%INT_MAX)

size_t sposx = 0;
size_t sposy = 0;

void drawbar()
  {
  printf("\e[%d;1H", ws.ws_col);//move to lowest row
  printf(bar, buf_name, cur, buf[cur], cap);//print bar and go back
  printf("msg: %s ", msg);
  printf("x: %zu ; y: %zu", sposx, sposy);
  }

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

void show_exit()
  {//terminate function
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &prevstate);//precisely what it looks like, sets terminal to previous state / mode
  printf("\e[40m\e[39m\e[2J\e[1;1H");//clears screen... and sets colors to normal
  }

void show_init(char** argv)
  {
  atexit(show_exit);
  prepare_terminal_normal_sa("show", &ws, &prevstate);
  strcpy(buf_name, argv[1]);
  if (!(main_buffer = shtex_create(buf_name, 256*256)))
    exit(1);
  }
