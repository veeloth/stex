#pragma once

#include "limo.c"
#include "mngr_t.c"
#include <string.h>

#define kur mngr_session->cursor
#define msg mngr_session->status
#define arg mngr_session->argument
#define current limo_session

struct limoptr limo_session = { 0 };
struct mngr* mngr_session;
char fbar[256];

int mngr_task_init(char* name, size_t size)
  {
  char ext[] = "mngr";
  size = size?size:65536;
  mngr_session = mngr_from(strlen(name), strlen(ext), name, ext, size);
  if (!mngr_session) return
    fprintf(stderr, "couldn't get a mngr"), 1;
  return 0;
  }
