#pragma once

#include <stdio.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <signal.h>

int cursor_save() { return printf("\e7"); }//save cursor position
int cursor_back() { return printf("\e8"); }//go back to where saved
int cursor_hide() { return printf("\e[?25l"); }
int cursor_show() { return printf("\e[?25h"); }
int cursor_here() { return (cursor_show(), cursor_save()); }
//#define cursor_draw() cursor_show(), cursor_back()//shows cursor and saves position

int cursor_line() { return printf("\e[6 q"); }
int cursor_undr() { return printf("\e[4 q"); }
int cursor_blok() { return printf("\e[2 q"); }

static struct winsize ws;
struct termios prevstate;
struct sigaction act;

void sa_handler_callback(int x) {  }

/*
 * ISIG disables ctrl-c ctrl+z
 * IEXTEN DISABLES ctrl+v and ctrl+o, which on some cases do smth
 * IXON disables ctrl+s ctrl+q
 * ICRNL fixes ctrl+m
 */
void enable_raw_mode(struct termios* prev)
  {
  struct termios raw;
  tcgetattr(STDIN_FILENO, &raw);
  *prev = raw;
  raw.c_iflag &= ~(IXON | ICRNL);
  raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
  }

void prepare_terminal(char* title, struct winsize* ws, struct termios* prev)
  {
  ioctl(STDOUT_FILENO, TIOCGWINSZ, ws);//can be used to get window size
  enable_raw_mode(prev);
  setbuf(stdout, NULL);//disables out-buf so printf() prints immediately
  printf("%c]0;%s%c", '\033', title, '\007');//sets window title to title
  }

void enable_raw_mode_normal_sa(struct termios* prev)
  {
  struct termios raw;
  tcgetattr(STDIN_FILENO, &raw);
  *prev = raw;
  raw.c_iflag &= ~(IXON | ICRNL);
  raw.c_lflag &= ~(ECHO | ICANON | IEXTEN);
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
  }

void prepare_terminal_normal_sa(char* title, struct winsize* ws, struct termios* prev)
  {
  ioctl(STDOUT_FILENO, TIOCGWINSZ, ws);
  enable_raw_mode_normal_sa(prev);
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


