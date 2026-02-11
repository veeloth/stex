#include <stdio.h>
#include "sh.c"

int main(int argc, char* argv[argc+1])
  {
  if (argc != 2)
    {
    printf("usage: re <name>");
    return 0;
    }
  struct tex* shm = shtex_create(argv[1], 0);
  printf("%s\n", shm->data);
  return 0;
  }
