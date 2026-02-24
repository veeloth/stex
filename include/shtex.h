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
  size_t kursor;//TODO: verify if this is valid when accessing
  size_t size;
  char snip[256];
  char sarg[256];
  char data[];
  };

void tex_init(struct tex* tex, size_t size)
  {
  tex->cursor = 0;
  tex->kursor = 0;
  tex->size = size;
  tex->snip[0] = 0;
  tex->sarg[0] = 0;
  tex->data[0] = 0;
  }

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
    if (MAP_FAILED == (ret = mmap
      (NULL, length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)))
      return perror("mapping failed"), ret;
    tex_init(ret, size);
    return ret;
    }

  //if file already existed, get the size
  stat(fpath, &fstats);
  length = fstats.st_size;

  if (MAP_FAILED == (ret = mmap
      (NULL, length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)))
    return perror("mapping failed"), ret;

  if (ret->size < ret->cursor)//TODO: verify kursor too
    return perror("invalid or corrupted data"), ret;
  return ret;
  }
