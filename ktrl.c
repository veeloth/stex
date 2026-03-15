#include <locale.h>
#include <unistd.h>

#include "include/ktrl.c"
#include "include/term.c"
#include "include/aux-draw.c"

#define IF STDIN_FILENO

void drop()
  {//terminate function
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &prevstate);//precisely what it looks like, sets terminal to previous state / mode
  }

void init(char** argv)
  {
  size_t size;
  char title[256];
  setlocale(LC_CTYPE, "");
  strcpy(self, argv[0]);
  strcpy(stex_name, argv[1]);
  sprintf(title, "ktrl %s", stex_name);

  bar[0] = 0;

  atexit(drop);
  ktrl_init(stex_name, sizeof(arg));
  prepare_terminal(title, &ws, &prevstate);

  sscanf(argv[2], "%zu", &size);
  if (task_init(stex_name, size)) exit(1);
  }


int main(int argc, char* argv[argc+1])
  {
  if (argc!=3) return
    fprintf(stderr, "usage: ktrl <name> <size>\n"), 1;
  init(argv);
  for(unsigned char in[4], l;;)
    l = read(IF, in, 4),
    ktrl(l, in),
    draw_bar();
  exit(0);
  }
