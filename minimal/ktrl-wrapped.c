#include "include/ktrl.c"
#define IF STDIN_FILENO

int main(int argc, char* argv[argc+1])
  {
  if (argc!=2) return perror("usage: ktrl <name>\n"), 1;
  ktrl_init(argv);
  for(unsigned char in[4], l;;)
    ktrl((l = read(IF, in, 4), l), in);
  exit(0);
  }
