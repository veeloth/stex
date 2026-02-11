#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sh.c"

int main(int argc, char* argv[argc+1])
  {
  if (argc != 3)
    {
    printf("usage: %s <name> \"<text>\"", argv[0]);
    return 0;
    }
  struct tex* shm = shtex_create(argv[1], strlen(argv[2]));
  for (size_t i; i < shm->size; ++i)
    shm->data[i] = argv[2][i];
  return 0;
  }
