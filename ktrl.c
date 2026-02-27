#include "include/ktrl.c"
#include "include/term.c"
#define IF STDIN_FILENO

char stex_name[256];

void drop()
  {//terminate function
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &prevstate);//precisely what it looks like, sets terminal to previous state / mode
  }

void init(char** argv)
  {
  size_t tmp;
  char title[256];
  mb_init();
  strcpy(self, argv[0]);
  strcpy(stex_name, argv[1]);
  sprintf(title, "ktrl %s", stex_name);

  atexit(drop);
  ktrl_init(stex_name, sizeof(arg));
  prepare_terminal(title, &ws, &prevstate);

  sscanf(argv[2], "%zu", &tmp);
  if (shm_typr_init(stex_name, tmp)) exit(1);
  }


int main(int argc, char* argv[argc+1])
  {
  if (argc!=3) return
    fprintf(stderr, "usage: ktrl <name> <size>\n"), 1;
  init(argv);
  for(unsigned char in[4], l;;)
    l = read(IF, in, 4),
    ktrl(l, in);
  exit(0);
  }
