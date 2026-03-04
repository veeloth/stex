#include "include/draw.c"
#include "include/ktrl.c"
#include "include/term.c"
#include <stdio.h>

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
  sprintf(title, "stex %s", stex_name);

  atexit(drop);
  ktrl_init(stex_name, sizeof(arg));
  prepare_terminal(title, &ws, &prevstate);
  draw_init(stex_name, ws.ws_row, ws.ws_col,
            cursor_save, cursor_back,
            cursor_hide, cursor_show,
            cursor_here);

  sscanf(argv[2], "%zu", &tmp);
  if (shm_typr_init(stex_name, tmp)) exit(1);
  }


int main(int argc, char* argv[argc+1])
  {
  if (argc!=3) return
    fprintf(stderr, "usage: stex <name> <size>"), 1;
  init(argv);
  unsigned char key[4], len;
  for(;;) len = read(IF, key, 4), ktrl(len, key), draw();
  exit(0);
  }
