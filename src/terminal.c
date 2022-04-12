
#ifndef TERMINAL_C
#define TERMINAL_C

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef __linux__  // linux includes
#include <sys/ioctl.h>
#include <unistd.h>
#elif _WIN32  // windows includes
#include <windows.h>
#else

#endif

struct Terminal {
  int width;
  int height;
};
struct Terminal TERMINAL;

bool Terminal_update_size() {
  int width = 0;
  int height = 0;
#ifdef __linux__
  // create a temporary place to store window size
  struct winsize w;
  // read size of terminal window into it
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

  width = (int)w.ws_col;
  height = (int)w.ws_row;
#elif _WIN32
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  int columns, rows;

  GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
  width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
  height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
#else
#endif

  // check if size changes at all
  if (TERMINAL.width != width || TERMINAL.height != height) {
    // update size, mark buffer needs re allocation
    TERMINAL.width = width;
    TERMINAL.height = height;

    return true;
  }
  return false;
}

void Terminal_clear() {
  printf("\e[H\e[2J\e[3J");
  // printf("\e[1;1H\e[2J");
}

struct Terminal Terminal_get() {
  return TERMINAL;
}

#endif
