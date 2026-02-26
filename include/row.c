#pragma once

#include <stddef.h>

/*check if pos is line continuation or beginning in str*/
#define line_beg(pos, str) (!pos|| str[pos-1]=='\n')
#define line_con(pos, str) (pos && str[pos-1]!='\n')
#define line_pre(pos, str) (str[pos] && str[pos]!='\n')
#define line_end(pos, str) (!str[pos]|| str[pos]=='\n')
#define line_mid(pos, str) (line_con(pos, str)&&line_pre(pos, str))

#include <stdio.h>
#include <unistd.h>
size_t row_end(size_t pos, char* str)
  {//returns first pos of current row
  while line_pre(pos, str) pos++;
  return pos;
  }

size_t row_start(size_t pos, char* str)
  {//returns first pos of current row
  while line_con(pos, str) pos--;
  return pos;
  }

size_t last_row(size_t pos, char* str)
  {//return first pos of last row
  if ((pos = row_start(pos, str)))
    return row_start(pos-1, str);
  return pos;
  }

size_t next_row(size_t pos, char* str)
  {//return first pos of last row
  if (str[pos = row_end(pos, str)])
      return pos+1;
  return pos;
  }
