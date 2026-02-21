#include <ctype.h>
#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "texbuf.c"

char command[512];
char self[258];
void constrained_move(int x);
void (*move)(int) = constrained_move;

void go(size_t x)
{ if (x <= strnlen(buf, cap)) cur = x; }

void constrained_move(int x)
{ go(cur + x); }

void raw_move(int x)
{ cur += x, cur %= cap; }

int detach(size_t x)
  {//shifts everything after cur by x bytes
  char* pos = buf+cur;//where we "are"; we'll move to the end of string, and start moving characters from there
  for (;*pos; ++pos) { }//go to end
  if (pos+x >= buf+cap) return sprintf(msg, "buffer full"), 1;//return if there's no room
  for (;pos >= buf+cur; --pos) pos[x] = pos[0];//move characters from pos x number of chars
  return 0;
  }

int attach(ptrdiff_t x)
  {//shifts everything after cur by x bytes
  char* pos = buf+cur+(x<0 ? -(x=-x) : 0);//where we "are";
  if (pos < buf || !*pos) return sprintf(msg, "nothing to delete"), 1;//return if there's no room
  do pos[0] = pos[x]; while (pos++[x]);
  return 0;
  }

size_t where(char* target)
  {//find word in text after start, wraps around
  if (*target) for (size_t i = 0, pos = cur+1;
    pos+i != cur;//if we're back at where we started, stop
    buf[pos+i]==target[i] ?//compare with target
    i++ : (pos+=i+1, i=0))//iff match compare next
    {
    if (!target[i]) return pos;//if all characters in target coincide, return position
    if (!buf[pos+i]) pos = 0;//wrap back to the start if at the end of data
    };
  return sprintf(msg, "%.s not found", target), cur;
  }

size_t last_word()
  {//returns index of last word's first character
  size_t pos = cur;
  while (!isalnum(buf[pos]) && pos>0) --pos;
  while (isalnum(buf[pos]) && pos>0) --pos;
  return pos;
  }

size_t next_word()
  {//returns index of next word's last character
  size_t pos = cur;
  while (!isalnum(buf[pos]) && buf[pos]) ++pos;
  while (isalnum(buf[pos]) && buf[pos]) ++pos;
  return pos;
  }

int delete(ptrdiff_t size)
{ return attach(-size) ? 1 : (move(size>0?-size:0), 0); }


int insert(char* src, size_t size)
  {
  if (detach(size)) return 1;
  for (int i = 0; i < size; ++i) buf[cur+i] = src[i];
  move(size);
  return 0;
  }

