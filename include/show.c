#include <limits.h>
#include <stdio.h>
#include <string.h>

#include "texbuf.c"
#include "term.c"

#define INT(x) (int)((x)%INT_MAX)

char stex_name[256];

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
  printf(bar, stex_name, cur, buf[cur], cap);//print bar and go back
  printf("msg: %s ", msg);
  }

void draw_msg()
  {
  printf("\e[30m\e[41m%.*s\e[0m", INT(sizeof(msg)), msg);
  }

void draw_arg()
  {
  cur_back();
  printf("\e[30m\e[43m%.*s\e[0m", INT(kur), arg);
  cur_here();
  printf("\e[30m\e[43m%s\e[0m", arg+kur);
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
  draw_bar(), draw_arg(), draw_msg(), cur_back();
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
  strcpy(stex_name, argv[1]);
  if (stex_init(stex_name, 0)) exit(1);
  }
