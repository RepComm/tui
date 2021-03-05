
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "./boolean.h"
#include "./utils.c"

#include "./tui.c"
#include "./terminal.c"
#include "./timer.c"

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

  tui->surface->strokeChar = '#';

  TimerP timer = Timer_create();
  bool doDrawLoop = true;

  IntervalP interval = Interval_create(1);

  int iw = term.getWidth();
  int ih = term.getHeight();

  while (doDrawLoop) {
    timer->step(timer);
    interval->calculate(interval, timer);

    if (interval->shouldIterate(interval)) {
      tui->surface->clear(tui->surface);

      tui->clear();

      float pw = iw/4;
      float ph = ih/4;

      float left = pw;
      float right = iw - pw;
      float top = ph;
      float bottom = ih - ph;

      // Draw a line to the surface
      tui->surface->strokeLine(
        tui->surface,
        left, top, right, ih/2
      );

      tui->surface->strokeLine(
        tui->surface,
        right, ih/2, left, bottom
      );

      tui->surface->strokeLine(
        tui->surface,
        left, top, left, bottom
      );

      //Put the screen buffer onto the terminal (stdout)
      fwrite(tui->screenBuffer, tui->screenBufferContentSize, 1, stdout);
      fflush(stdout);
    }
  }
  return 0;
}
