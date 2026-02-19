#include "include/show.c"

int main(int argc, char* argv[argc+1])
  {
  if (argc!=2) return perror("usage: show <name>"), 1;
  show_init(argv);
  for(;;) draw(), usleep(16700);
  exit(0);
  }
