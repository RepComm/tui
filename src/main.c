
#include <stdbool.h>
#include <stdio.h>

#include "./terminal.c"
// #include "./tui.c"
#include "./rect.c"
#include "./surface.c"
#include "./timer.c"

int main(int argc, char **argv) {
  printf("Hello World");
  TimerP timer = Timer_create();
  IntervalP interval = Interval_create(5);

  bool shouldLoop = true;

  SurfaceP canvas = Surface_create();
  char edit[100] = "hello world";
  int editSize = 4;

  // continuous loop
  while (shouldLoop) {
    // keep track of time
    Timer_step(timer);

    // check if its time to render
    if (Interval_calculate(interval, timer)) {
      Terminal_clear();

      // check if we need to update size
      if (Terminal_update_size()) {
        // printf ("Size changed to %d by %d\n", TERMINAL.width,
        // TERMINAL.height);
        Surface_setSize(canvas, (float)TERMINAL.width, (float)TERMINAL.height);
      }
      float w = (float)TERMINAL.width;
      float h = (float)TERMINAL.height;

      canvas->strokeLine(canvas, 0.0, 0.0, w, h);

      // printf to canvas built in 'line' utility string variable
      float textWidth = (float)snprintf(canvas->line, canvas->lineLength,
                                        "Width : %d, Height : %d",
                                        TERMINAL.width, TERMINAL.height);

      // draw canvas->line in center of screen
      Surface_drawText(canvas, w / 2 - textWidth / 2, h / 2, canvas->line);

      

      if (editSize > 1) {
        // draw canvas->line in center of screen
        Surface_drawText(canvas, w / 2 - textWidth / 2, (h / 2) + 2.0, edit);
      }

      // write buffer to terminal
      fwrite(canvas->buffer, canvas->buffersize - 1, 1, stdout);
      fflush(stdout);
    }
  }
}
