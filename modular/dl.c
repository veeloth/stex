#include <stdio.h>
#include <sys/mman.h>

int main(int argc, char* argv[argc+1])
  {
  if (argc!=2)
  { printf("usage: %s <name>\n", argv[0]); return 0; }
  shm_unlink(argv[1]);
  return 0;
  }
