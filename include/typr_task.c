#pragma once

#include <string.h>
#include "typr.c"

#define mcr main_typr->cursor
#define cap main_typr->size
#define buf main_typr->data

struct typr* main_typr;

int typr_task_init(char* name, size_t size)
  {
  char ext[] = "typr";
  size = size?size:65536;
  main_typr = typr_from(strlen(name), strlen(ext), name, ext, size);
  if (!main_typr) return
    fprintf(stderr, "couldn't get a typr"), 1;
  return 0;
  }

