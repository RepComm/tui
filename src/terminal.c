
#ifndef TERMINAL_C
#define TERMINAL_C

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>

struct Terminal {
  int width;
  int height;
};
struct Terminal TERMINAL;

bool Terminal_ioctl_update_size() {
  //create a temporary place to store window size
  struct winsize w;
  //read size of terminal window into it
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

  //check if size changes at all
  if (TERMINAL.width != w.ws_col ||
      TERMINAL.height != w.ws_row) {

    printf ("lines %d\n", w.ws_row);
    printf ("columns %d\n", w.ws_col);

    //update size, mark buffer needs re allocation
    TERMINAL.width = (int) w.ws_col;
    TERMINAL.height = (int) w.ws_row;

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
