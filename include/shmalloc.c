#pragma once

#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdio.h>

/*
  returns NULL in case of error, otherwise shm object and
  sets its first byte to zero if it wasn't created already
 */

void* shmalloc(char name[256], size_t size)
  {
  const int flags = O_CREAT | O_RDWR;
  const int mode = S_IRUSR | S_IWUSR;
  const int prot = PROT_READ | PROT_WRITE;

  char fpath[265]; sprintf(fpath, "/dev/shm/%s", name);
  int fexists = !access(fpath, F_OK);
  int fd = shm_open(name, flags, mode);
  void* ret = mmap(NULL, size, prot, MAP_SHARED, fd, 0);

  if (fd == -1) return
    fprintf(stderr, "file opening failed"), NULL;
  if (!fexists && ftruncate(fd, size) == -1 &&
      !(0[(char*)ret] = 0)) return
      fprintf(stderr, "size setting failed"), NULL;
  if (ret == MAP_FAILED) return
    perror("mapping failed"), NULL;
  return ret;
  }
