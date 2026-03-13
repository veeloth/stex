#pragma once

#include <stddef.h>

#include <unistd.h>
#include <stdint.h>
#include "shmalloc.c"

#define VERSION 1

struct mngr
  {
  uint16_t vers;
  size_t cursor;
  char status[256];
  char argument[256];
  };

void mngr_init(struct mngr* mngr, size_t size)
  {
  mngr->vers = VERSION;
  mngr->cursor =
  mngr->status[0] =
  mngr->argument[0] = 0;
  }

int mngr_valid(char name[256], struct mngr* mngr)
  {/*true iff mngr is valid*/
  char errmsg[] = "%s version: %hu\ncurrent version: %hu";
  if (mngr->vers!=VERSION) return
    fprintf(stderr, errmsg, name, mngr->vers, VERSION), 0;
  if (mngr->cursor >= sizeof(mngr->argument))
    return fprintf(stderr, "invalid mngr"), 0;
  return 1;
  }

//TODO: abstract this, this function can be abstracted
//besides, it doesn't belong in here
struct mngr* mngr_from(size_t name_len, size_t ext_len,
                  char name[name_len], char ext[ext_len],
                  size_t str_len)
  {/*returns mngr with identified by name*/
  if (name_len + ext_len + 1 > 255) return NULL;
  char mngr_name[256];
  sprintf(mngr_name, "%s.%s", name, ext);
  size_t mngr_size = sizeof(struct mngr) + str_len;
  struct mngr* ret = shmalloc(mngr_name, mngr_size);
  if (!ret) return NULL;
  if (ret->vers) return mngr_valid(name, ret)?ret:NULL;
  else mngr_init(ret, str_len);
  return ret;
  }
