#pragma once

#include <locale.h>
#include <stdlib.h>
#include <stddef.h>

#include "texbuf.c"

#define utfbeg(c) (c&0xC0^0x80)
#define utfcon(c) (!(c&0xC0^0x80))

struct mb { char str[4]; };

struct mb mb(char* c)
  {
  return (struct mb)
    {
    c[0],
    utfcon(c[1])?c[1]:0,
    utfcon(c[2])?c[2]:0,
    utfcon(c[3])?c[3]:0,
    };
  }

void mb_init()
  {
  setlocale(LC_CTYPE, "");
  }

size_t mbc(size_t c, ptrdiff_t x)
  {//ret distance to next code point in direction x
  for(;mblen(buf+c+x, 256) < 0; x>0?x++:x--);
  return x>0?x:-x;
  }

ptrdiff_t mb_get(size_t c, ptrdiff_t x)
  {//move c x multibyte characters in buf and ret c-nc
  ptrdiff_t nx = 0, pol = x>0?1:-1;
  if (x) for (;x != 0; x-=pol)
    nx+=mbc(c, nx+pol)*pol;
  return nx;
  }

size_t mb_add(size_t a, ptrdiff_t x)
  {//move c x multibyte characters in buf and ret c-nc
  ptrdiff_t nx = 0, pol = x>0?1:-1;
  if (x) for (;x != 0; x-=pol)
    //nx+=1*pol;
    //sprintf(msg, "%zu %zu ", nx, mbc(a, nx+pol)),
    nx=mbc(a, nx+pol)*pol;
    //sprintf(msg, "%zu\n", nx);
  return a+nx;
  }

ptrdiff_t mb_sub(size_t b, size_t a)
  {//count mbcs from a to b / ret b - a
  ptrdiff_t x = 0, pol = a<b?1:-1;
  if (a<b) for (;a+x<b;) x = mbc(a, x+1);
  else for (;a+x>b;) x = mbc(a, x-1);
  return x;
  }


