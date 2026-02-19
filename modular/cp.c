#include <stdio.h>
#include <string.h>
#include "shtex.h"

int main(int argc, char* argv[argc+1])
  {
  if (argc!=3)
    return perror("usage: cp <src> <dst>\n"), 1;
  struct tex* src = shtex_create(argv[1], 1);
  struct tex* dst = shtex_create(argv[2], src->size);
  memcpy(dst, src, src->size);
  return 0;
  }
