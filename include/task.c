#pragma once

#include "mngr_task.c"
#include "typr_task.c"

int task_init(char* name, size_t size)
  {
  if (typr_task_init(name, size)) return 1;
  if (mngr_task_init(name, size)) return 1;
  current.cursor = &main_typr->cursor;
  current.buffer =
  current.line = main_typr->data;
  current.size = &main_typr->size;
  return 0;
  }
