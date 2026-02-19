#include <stdio.h>
#include "sh.c"

int main(int argc, char* argv[argc+1])
  {
  if (argc != 2) return perror("usage: re <name>"), 1;
  struct tex* shm = shtex_create(argv[1], 0);
  printf("e %c %s\n", shm->data[0], shm->data);
  return 0;
  }
