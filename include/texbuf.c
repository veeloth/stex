#pragma once

#include <string.h>
#include <stddef.h>
#include "tex.c"

#define mcr main_typr->cursor
#define kur main_typr->kursor
#define cap main_typr->size
#define msg main_typr->snip
#define arg main_typr->sarg
#define buf main_typr->data

struct typr* main_typr;
size_t* current_cursor;
size_t* current_size;
char* current_str;


int shm_typr_init(char* name, size_t size)
  {
  char ext[] = "typr";
  size = size?size:65536;
  main_typr = typr(strlen(name), strlen(ext),  name, ext, size);
  if (!main_typr) return
    fprintf(stderr, "couldn't get a typr"), 1;
  current_cursor = &main_typr->cursor;
  current_size = &main_typr->size;
  current_str = main_typr->data;
  return 0;
  }

