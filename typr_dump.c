#include "include/typr.c"
#include <string.h>

int main(int argc, char* argv[argc+1])
  {
  if (argc!=2) return
    fprintf(stderr, "usage: dump <name>\n"), 1;

  char typr_name[256];
  sprintf(typr_name, "%s.%s", argv[1], "typr");
  size_t typr_size = sizeof(struct typr) + 1;
  struct typr* typr = shmalloc(typr_name, typr_size);
  printf("vers: %hu\ncursor: %zu\nsize: %zu", typr->vers, typr->cursor, typr->size);

  return 0;
  }
