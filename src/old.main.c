
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

  int widthCurrent = 32;
  int heightCurrent = 32;
  int widthPrevious = 31;
  int heightPrevious = 31;

  while (doDrawLoop) {
    Timer_step(timer);
    
    interval->calculate(interval, timer);

    if (interval->shouldIterate(interval)) {
      widthCurrent = term.getWidth();
      heightCurrent = term.getHeight();
      if (widthCurrent != widthPrevious || heightCurrent != heightPrevious) {
        tui->setSize(tui, (float) widthCurrent, (float) heightCurrent);
        widthPrevious = widthCurrent;
        heightPrevious = heightCurrent;
      }

      tui->surface->clear(tui->surface);

      tui->clear();

      float pw = widthCurrent/4;
      float ph = heightCurrent/4;

      float left = pw;
      float right = widthCurrent - pw;
      float top = ph;
      float bottom = heightCurrent - ph;

      // Draw a line to the surface
      tui->surface->strokeLine(
        tui->surface,
        left, top, right, heightCurrent/2
      );

      tui->surface->strokeLine(
        tui->surface,
        right, heightCurrent/2, left, bottom
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
