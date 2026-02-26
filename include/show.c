#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "texbuf.c"
#include "term.c"
#include "row.c"

#define DEF_BAR "%1$s[%2$zu]: %3$08hhb %3$02hhx %3$d  size: %4$zu "
#define INT(x) (int)((x)%INT_MAX)

char stex_name[256];
char fbar[256] = DEF_BAR;
char bar[256] = { };

void printmb(char* str, size_t* pos)
  {
  int l = mblen(str+*pos, 6);
  if (l < 0) return (void)
    printf("\e[30;41m%02hhx\e[0m", str[*pos]),
    (void)(*pos += 1);
  printf("%.*s", l, str+*pos);
  *pos += l;
  }

size_t draw_row(size_t pos)
  {//pos < ws.ws_col is faulty, doesn't account for multibyte chars
  size_t i = 0;
  while (buf[pos] && i<ws.ws_col && buf[pos]!='\n')
    pos==mcr && cur_here(),
    printmb(buf, &pos),i++;
  return pos;
  }

size_t draw_row_wrapless(char* row)
  {
  size_t i = 0;
  while (row[i] && row[i]!='\n')
    if (i==ws.ws_col-1) return putchar('>'), i;
    else printmb(row, &i);
  return i;
  }

void draw_bar()
  {
  sprintf(bar, fbar, stex_name, mcr, buf[mcr], cap);
  sprintf(bar+strlen(bar), "msg: %s ", msg);
  printf("\e[%d;1H", ws.ws_row);//move to lowest row
  draw_row_wrapless(bar);
  }

void draw_msg()
  {
  cur_back();
  printf("\e[30m\e[41m%.*s\e[0m", INT(sizeof(msg)), msg);
  }

void draw_arg()
  {
  if (!*arg) return;
  cur_back();
  printf("\e[30m\e[43m%.*s\e[0m", INT(kur), arg);
  cur_here();
  printf("\e[30m\e[43m%s\e[0m", arg+kur);
  }

size_t screenpos()
  {
  size_t pos = mcr;
  for (size_t i = 0; i < ws.ws_row/2; i++)
    pos = last_row(pos, buf);
  return pos;
  }

void draw()
  {//UI / printing function
  cur_hide();//cursor hidden by default
  printf("\e[40m\e[39m \e[2J");//set color to normal and clear screen
  printf("\e[1;1H");//go back to beginning of screen
  size_t pos = screenpos(), row = 1;
  for (; row <= ws.ws_row-1; row++)
    {
    printf("\e[%1$zu;1H", row);//go to corresponding row
    pos = draw_row(pos);
    if (pos==mcr) cur_here();
    if (!buf[pos]) break;//if end of buffer stop drawing
    if (buf[pos]=='\n') pos++;
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
