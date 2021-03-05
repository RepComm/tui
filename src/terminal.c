
#ifndef TERMINAL_C
#define TERMINAL_C

#include <stdlib.h>
#include <stdio.h>
#include "./boolean.h"

struct Terminal {
  bool initialized;
  int (*getWidth)();
  int (*getHeight)();
  void (*clear)();
  int cachedWidth;
  int cahcedHeight;
};
struct Terminal _Terminal_SINGLETON;

int Terminal_getWidth () {
  char * data = getenv("COLUMNS");
  if (data == 0) {
    printf("Could not get columns");
    exit(-1);
  }
  return atoi(data);
}

int Terminal_getHeight (){
  return atoi(getenv("LINES"));
}

void Terminal_clear () {
  printf("\e[1;1H\e[2J");
}

struct Terminal Terminal_get () {
  if (!_Terminal_SINGLETON.initialized) {
    printf("initialized terminal\n");
    _Terminal_SINGLETON.initialized = true;
    _Terminal_SINGLETON.clear = &Terminal_clear;
    _Terminal_SINGLETON.getHeight = &Terminal_getHeight;
    _Terminal_SINGLETON.getWidth = &Terminal_getWidth;

    _Terminal_SINGLETON.cachedWidth = 80;
    _Terminal_SINGLETON.cahcedHeight = 40;
  }
  return _Terminal_SINGLETON;
}

#endif
