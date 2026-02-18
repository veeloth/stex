#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "shtex.h"

#define buf main_buffer->data
#define cur main_buffer->cursor
#define cap main_buffer->size

struct tex* main_buffer;
char bufname[256];
char command[512];
char status[512];
char self[258];
void (*move)(int);

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
  if (pos+x >= buf+cap) return sprintf(status, "buffer full"), 1;//return if there's no room
  for (;pos >= buf+cur; --pos) pos[x] = pos[0];//move characters from pos x number of chars
  return 0;
  }

int attach(size_t x)
  {//shifts everything after cur by x bytes
  char* pos = buf+cur-x;//where we "are";
  if (pos < buf) return sprintf(status, "already at beginning"), 1;//return if there's no room
  do pos[0] = pos[x]; while (pos++[x]);
  //for (;pos[x]; ++pos) pos[0] = pos[x];//move characters to pos x number of chars
  return 0;
  }

size_t where(char* word)
  {//find word in text after start, wraps around
  if (*word)
  for (size_t i = 0, pos = cur+1; pos+i != cur;
       buf[pos+i]==word[i]? i++ : (pos+=i+1, i=0))//compare target and text
    {
    if (!word[i]) return pos;//if all characters in target coincide, return position
    if (!buf[pos+i]) pos = 0;//wrap back to the start if at the end of data
    };
  return sprintf(status, "%.s not found", word), cur;
  }


int delete(size_t size)
{ return attach(size) ? 1 : (move(-size), 0); }


int insert(char* src, size_t size)
  {
  if (detach(size)) return 1;
  for (int i = 0; i < size; ++i) buf[cur+i] = src[i];
  move(size);
  return 0;
  }

void search()
  {
  char* snip;//buffer containing whatever user wants to search for
  char snipname[256];

  sprintf(snipname, "%s.snip", bufname);
  snip = shtex_create(snipname, 256)->data;

  sprintf(command, "%s %s", self, snipname);
  system(command);

  go(where(snip));
  shm_unlink(snipname);
  }


