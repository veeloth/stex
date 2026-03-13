#pragma once

#include <stddef.h>
#include <stdint.h>

struct limo
  {//TODO: implement init functions
  uint16_t vers;
  size_t cursor;
  size_t* size;
  char* buffer;
  char* line;
  };

struct limoptr
  {
  size_t* cursor;
  size_t* size;
  char* buffer;
  char* line;
  };
