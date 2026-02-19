#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct tex
  {
  size_t cursor;
  size_t size;
  char snip[256];
  char data[];
  };

struct tex* shtex_create(char name[256], size_t size)
  {
  struct tex* ret = NULL;
  struct stat fstats;
  size_t length = sizeof(struct tex) + size;
  char fpath[265] = "/dev/shm/"; strcat(fpath, name);
  int fexists = !access(fpath, F_OK);
  int fd;

  if ((fd = shm_open(name, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR)) == -1)
    return perror("file opening failed"), ret;

  if (!fexists)//only if file DIDN'T exist already (that is, if someone else created it already)
    {//set file's length
    if (ftruncate(fd, length) == -1) return perror("size setting failed"), ret;
    if ((ret = mmap(NULL, length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED)
      return perror("mapping failed"), ret;
    ret->cursor = 0;
    ret->snip[ret->cursor] = ret->data[ret->cursor] = 0;
    ret->size = size;
    return ret;
    }

  //if file already existed, get the size
  stat(fpath, &fstats);
  length = fstats.st_size;

  if ((ret = mmap(NULL, length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED)
    return perror("mapping failed"), ret;

  if (ret->size < ret->cursor)
    return perror("invalid or corrupted data"), ret;
  return ret;
  }
