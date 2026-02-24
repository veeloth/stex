#pragma once

#include "shtex.h"
#include <stddef.h>

#define BAR "%1$s[%2$zu]: %3$08hhb %3$02hhx %3$d  size: %4$zu "
#define cur *main_cursor
#define kur main_buffer->kursor
#define cap *main_size
#define msg main_buffer->snip
#define arg main_buffer->sarg
#define buf main_data

struct tex* main_buffer;
size_t* main_cursor;
size_t* main_size;
char* main_data;
char bar[256] = BAR;

int stex_init(char* name, size_t size)
  {
  main_buffer = shtex_create(name, size?size:65536);
  if (!main_buffer) return
    fprintf(stderr, "couldn't create buffer"), 1;
  main_cursor = &main_buffer->cursor;
  main_size = &main_buffer->size;
  main_data = main_buffer->data;
  return 0;
  }

