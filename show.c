#include "include/term.c"
#include "include/draw.c"
#include <signal.h>
#include <string.h>

char stex_name[256];

void show_exit()
  {//terminate function
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &prevstate);//precisely what it looks like, sets terminal to previous state / mode
  printf("\e[0m\e[2J\e[1;1H");//clears screen... and sets colors to normal
  }

void int_handler(int n) { show_exit(), exit(0); }

void show_init(char** argv)
  {
  char title[256];
  atexit(show_exit);
  signal(SIGINT, int_handler);
  strcpy(stex_name, argv[1]);
  sprintf(title, "show %s", stex_name);
  prepare_terminal_normal_sa(title, &ws, &prevstate);
  if (shm_typr_init(stex_name, 0)) exit(1);
  draw_init(ws.ws_row, ws.ws_col,
            cursor_save, cursor_back,
            cursor_hide, cursor_show,
            cursor_here);
  }

int main(int argc, char* argv[argc+1])
  {
  if (argc!=2) return perror("usage: show <name>"), 1;
  show_init(argv);
  for(;;) draw(), usleep(16700);
  exit(0);
  }
