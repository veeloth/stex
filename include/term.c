#pragma once

#include <stdio.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <signal.h>

#define cur_save() printf("\e7")//save cursor position
#define cur_back() printf("\e8")//go back to where saved
#define cur_hide() printf("\e[?25l")
#define cur_show() printf("\e[?25h")
#define cur_here() (cur_show(), cur_save())
//#define cur_draw() cur_show(), cur_back()//shows cursor and saves position

#define cur_line() printf("\e[6 q")
#define cur_undr() printf("\e[4 q")
#define cur_blok() printf("\e[2 q")

struct winsize ws;
struct termios prevstate;
struct sigaction act;

void sa_handler_callback(int x) {  }

void enable_raw_mode(struct termios* prev)
  {
  struct termios raw;
  tcgetattr(STDIN_FILENO, &raw);
  *prev = raw;
  raw.c_lflag &= ~(ECHO | ICANON);
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
  }

void prepare_terminal(char* title, struct winsize* ws, struct termios* prev)
  {
  act.sa_handler = sa_handler_callback;
  sigaction(SIGINT, &act, NULL);
  ioctl(STDOUT_FILENO, TIOCGWINSZ, ws);//can be used to get window size
  enable_raw_mode(prev);
  setbuf(stdout, NULL);//disables out-buf so printf() prints immediately
  printf("%c]0;%s%c", '\033', title, '\007');//sets window title to title
  }

void prepare_terminal_normal_sa(char* title, struct winsize* ws, struct termios* prev)
  {
  ioctl(STDOUT_FILENO, TIOCGWINSZ, ws);
  enable_raw_mode(prev);
  setbuf(stdout, NULL);
  printf("%c]0;%s%c", '\033', title, '\007');
  }

void get_cur(size_t* x, size_t* y)
  {
  char buffer[32];
  size_t i = 0;
  printf("\e[6n");
  for (;i < sizeof(buffer) - 1; i++) if (
      read(STDIN_FILENO, &buffer[i], 1),
      buffer[i] == 'R') break;
  buffer[i] = 0;
  sscanf(buffer+2, "%zu;%zu", x, y);
  }


