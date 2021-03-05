
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "./boolean.h"
#include "./utils.c"

#include "./tui.c"
#include "./terminal.c"

int main(int argc, char **argv) {
  //Helps with clear, width and height
  struct Terminal term = Terminal_get();

  //Create a terminal UI, supplying a clear screen function callback
  TUIDataP tui = TUI_create(term.clear);

  //Set the size
  tui->setSize(tui, (float) term.getWidth(), (float) term.getHeight());
  
  //Render to its screen buffer (char *)
  tui->render(tui);

  //Create two points  
  Vec2P from = Vec2_create();
  Vec2P to = Vec2_create();
  to->set(to, 20, 5);

  //Draw a line to the surface
  tui->surface->strokeLine(tui->surface, from, to);

  //Put the screen buffer onto the terminal (stdout)
  fwrite(tui->screenBuffer, tui->screenBufferContentSize, 1, stdout);

  return 0;
}
