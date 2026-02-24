#include <limits.h>
#include <stdio.h>
#include <string.h>

#include "texbuf.c"
#include "term.c"

#define INT(x) (int)((x)%INT_MAX)

size_t draw_row(size_t pos)
  {//pos < ws.ws_col is faulty, doesn't account for multibyte chars
  size_t opos = pos;
  while (buf[pos] && pos-opos<ws.ws_col && buf[pos]!='\n')
    pos==cur && cur_here(),
    putchar(buf[pos++]);
  return pos;
  }

void draw_bar()
  {
  printf("\e[%d;1H", ws.ws_row);//move to lowest row
  printf(bar, buf_name, cur, buf[cur], cap);//print bar and go back
  printf("msg: %s ", msg);
  }

void draw_msg()
  {
  cur_back();
  printf("\e[30m\e[41m%.*s\e[0m", INT(sizeof(msg)), msg);
  }

void draw()
  {//UI / printing function
  cur_hide();//cursor hidden by default
  printf("\e[40m\e[39m \e[2J");//set color to normal and clear screen
  printf("\e[1;1H");//go back to beginning of screen
  for (size_t pos = 0, row = 1; row <= ws.ws_row-1; row++)
    {
    printf("\e[%1$zu;1H", row);//go to corresponding row
    pos = draw_row(pos);
    if (pos==cur) cur_here();
    if (!buf[pos++]) break;//if end of buffer stop drawing
    }
  draw_bar();
  draw_msg();
  cur_back();//move cursor to its position on screen
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
