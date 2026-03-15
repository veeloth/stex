#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "task.c"
#include "row.c"

#define mcr main_typr->cursor
#define cap main_typr->size
#define buf main_typr->data

#define INT(x) (int)((x)%INT_MAX)
#define rows draw_state.global_rows
#define cols draw_state.global_cols
#define stex_name draw_state.global_stex_name


struct draw_globals
  {
  unsigned short int global_rows;
  unsigned short int global_cols;
  char global_stex_name[256];
  };
struct draw_globals draw_state;


int (*cur_save)(void);
int (*cur_back)(void);
int (*cur_hide)(void);
int (*cur_show)(void);
int (*cur_here)(void);

/*you need to call this function and provide callbacks*/
void draw_init(char* name, unsigned short int row, unsigned short int col, int (*save)(), int(*back)(), int (*hide)(), int (*show)(), int (*here)())
  {
  strcpy(stex_name, name);
  rows = row;
  cols = col;
  cur_save = save;
  cur_back = back;
  cur_hide = hide;
  cur_show = show;
  cur_here = here;
  }

void printmb(char* str, size_t* pos)
  {
  int l = mblen(str+*pos, 6);
  if (l < 0) return (void)
    printf("\e[30;41m%02hhx\e[0m", str[(*pos)++]);
  printf("%.*s", l, str+*pos);
  *pos += l;
  }

size_t draw_row(size_t pos)
  {//pos < cols is faulty, doesn't account for multibyte chars
  size_t i = 0;
  while (buf[pos] && i<cols && buf[pos]!='\n')
    pos==mcr && cur_here(),
    printmb(buf, &pos),i++;
  return pos;
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
  for (size_t i = 0; i < rows/2; i++)
    pos = last_row(pos, buf);
  return pos;
  }

void draw()
  {//UI / printing function
  cur_hide();//cursor hidden by default
  printf("\e[40m\e[39m \e[2J");//set color to normal and clear screen
  printf("\e[1;1H");//go back to beginning of screen
  size_t pos = screenpos(), row = 1;
  for (; row <= rows-1; row++)
    {
    printf("\e[%1$zu;1H", row);//go to corresponding row
    pos = draw_row(pos);
    if (pos==mcr) cur_here();
    if (!buf[pos]) break;//if end of buffer stop drawing
    if (buf[pos]=='\n') pos++;
    }
  draw_arg(), draw_msg(), cur_back();
  }

#undef rows
#undef cols
#undef stex_name
#undef mcr
#undef cap
#undef buf
