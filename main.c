#include "include/show.c"
#include "include/ktrl.c"

#define IF STDIN_FILENO

int main(int argc, char* argv[argc+1])
  {
  if (argc!=3) return
    fprintf(stderr, "usage: stex <name> <size>"), 1;
  ktrl_init(argv);
  unsigned char key[4], len;
  for(;;) len = read(IF, key, 4), ktrl(len, key), draw();
  exit(0);
  }
