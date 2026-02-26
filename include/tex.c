#pragma once

#include <unistd.h>
#include <stdint.h>
#include "shmalloc.c"

#define VERSION 1

struct typr
  {
  uint16_t vers;
  size_t cursor;
  size_t kursor;
  size_t size;
  char snip[256];
  char sarg[256];
  char data[];
  };

void typr_init(struct typr* typr, size_t size)
  {
  typr->vers = VERSION;
  typr->size = size;
  typr->cursor =
  typr->kursor =
  typr->snip[0] =
  typr->sarg[0] =
  typr->data[0] = 0;
  }

int typr_valid(char name[256], struct typr* typr)
  {/*true iff typr is valid*/
  char errmsg[] = "%s version: %hu\ncurrent version: %hu";
  if (typr->vers!=VERSION) return
    fprintf(stderr, errmsg, name, typr->vers, VERSION), 0;
  if (typr->kursor >= sizeof(typr->sarg)
      || typr->cursor >= typr->size)
    return fprintf(stderr, "invalid typr"), 0;
  return 1;
  }

struct typr* typr(size_t name_len, size_t ext_len,
                  char name[name_len], char ext[ext_len],
                  size_t str_len)
  {/*returns typr with identified by name*/
  if (name_len + ext_len + 1 > 255) return NULL;
  char typr_name[256];
  sprintf(typr_name, "%s.%s", name, ext);
  size_t typr_size = sizeof(struct typr) + str_len;
  struct typr* ret = shmalloc(typr_name, typr_size);
  if (!ret) return NULL;
  if (ret->vers) return typr_valid(name, ret)?ret:NULL;
  else typr_init(ret, str_len);
  return ret;
  }
