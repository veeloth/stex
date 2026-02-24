#pragma once

#include "texbuf.c"
#include <stdlib.h>
#include <stddef.h>

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


