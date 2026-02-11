#include <sys/ioctl.h>
#include <termios.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <stdbool.h>
#include <string.h>
//#include "sh.c"

static bool done = false;
struct sigaction act;
struct termios prev_state;
unsigned short blen;//buffer len
struct winsize ws;

int main (int argc, char* argv[argc + 1])
    {
    if (argc != 2)
      {
      printf("usage: show <name>");
      return 0;
      }
    //setup();
    char command[261] = "./re ";
    strcat(command, argv[1]);
    printf("\e[2J");
    while(!done)
        {
	printf("\e[0;1H");//goes to 0 0 and clears the screen
	fflush(stdout);
	system(command);
	fflush(stdout);
        }
    printf("\e[2J");
    //tcsetattr(STDIN_FILENO, TCSAFLUSH, &prev_state);
    return 0;
    }
