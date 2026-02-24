#pragma once

#include "shtex.h"

#define BAR "%1$s[%2$zu]: %3$02x %3$d  size: %4$zu "
#define cur main_buffer->cursor
#define cap main_buffer->size
#define kur main_buffer->scur
#define msg main_buffer->snip
#define arg main_buffer->sarg
#define buf main_buffer->data

struct tex* main_buffer;
char buf_name[256];
char bar[256] = BAR;
