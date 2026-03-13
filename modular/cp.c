#include <fcntl.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include "shmalloc.c"

int main(int argc, char* argv[argc+1])
  {
  if (argc!=3)
    return fprintf(stderr, "usage: cp <src> <dst>\n"), 1;
  struct stat info;
  int fd = shm_open(argv[1], O_RDONLY, S_IRUSR);
  if (fstat(fd, &info))
    return fprintf(stderr, "stat error\n"), 1;
  char* src = shmalloc(argv[1], info.st_size);
  char* dst = shmalloc(argv[2], info.st_size);
  memcpy(dst, src, info.st_size);
  return 0;
  }
