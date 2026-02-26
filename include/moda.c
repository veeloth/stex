#include <ctype.h>
#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "texbuf.c"
#include "row.c"
#include "mb.c"

#define cur *current_cursor
#define siz *current_size
#define str current_str

char command[512];
char self[258];


void go(size_t x)
{ if (x <= strnlen(str, siz)) cur = x; }

void raw_move(ptrdiff_t x)
{ cur += x, cur %= siz; }

void constrained_move(ptrdiff_t x)
{ go(cur + x); }

void mb_move(ptrdiff_t x)
{ constrained_move(mb_get(cur, x)); }

/*
  saves x coordinate of cursor
  and goes to beginning of previous line + x
 *
  uses 2 variables to count x
  mbx representing x in multibyte characters 
  pos representing current position in bytes (chars)
 *
  first we count x:
  decrement pos til pos==0 or we reach a '\n'
  incrementing x each start of mbc
 *
  then we go back:
  increment pos til x==0,
  decrementing x each start of mbc
 */
void line_up()
  {
  size_t mbx = 0, pos = cur;
  while line_con(pos, str) if utfbeg(str[pos--]) mbx++;
  pos = last_row(cur, str);
  while (mbx-- && str[pos]!='\n')
    pos+=mblen(str+pos, 6);
  go(pos);
  }

void line_down()
  {
  size_t mbx = 0, pos = cur;
  while line_con(pos, str) if utfbeg(str[pos--]) mbx++;
  pos = next_row(cur, str);
  while (mbx-- && str[pos]!='\n')
    pos+=mblen(str+pos, 6);
  go(pos);
  }

int detach(size_t x)
  {//shifts everything after cur by x bytes
  char* pos = str+cur;//where we "are"; we'll move to the end of string, and start moving characters from there
  while (*pos) ++pos;//go to end
  if (pos+x >= str+siz) return//return if there's no room
    sprintf(msg, "buffer full"), 1;
  for (;pos >= str+cur; --pos) pos[x] = pos[0];//move characters from pos x number of chars
  return 0;
  }

int attach(ptrdiff_t x)
  {//shifts everything after cur by x bytes
  char* pos = str+cur+(x<0 ? -(x=-x) : 0);//where we "are";
  if (pos < str || !*pos) return//return if there's no room
    sprintf(msg, "nothing to delete"), 1;
  do pos[0] = pos[x]; while (pos++[x]);
  return 0;
  }

size_t where(char* target)
  {//find word in text after start, wraps around
  if (*target) for (size_t i = 0, pos = mcr+1;
    pos+i != mcr;//if we're back at where we started, stop
    buf[pos+i]==target[i] ?//compare with target
    i++ : (pos+=i+1, i=0))//iff match compare next
    {
    if (!target[i]) return pos;//if all characters in target coincide, return position
    if (!buf[pos+i]) pos = 0;//wrap back to the start if at the end of data
    };
  return sprintf(msg, "%.s not found", target), cur;
  }

size_t prev_word()
  {//returns index of last word's first character
  size_t pos = cur;
  while (!isalnum(str[pos]) && pos>0) --pos;
  while (isalnum(str[pos]) && pos>0) --pos;
  return pos;
  }

size_t next_word()
  {//returns index of next word's last character
  size_t pos = cur;
  while (!isalnum(str[pos]) && str[pos]) ++pos;
  while (isalnum(str[pos]) && str[pos]) ++pos;
  return pos;
  }

int delete(ptrdiff_t size)
{ return attach(size) ? 1 :
  (constrained_move(size<0?size:0), 0); }

int mb_delete(ptrdiff_t size)
{ return delete(mb_get(cur, size)); }

int insert(char* src, size_t size)
  {
  if (detach(size)) return 1;
  for (int i = 0; i < size; ++i) str[cur+i] = src[i];
  constrained_move(size);
  return 0;
  }

