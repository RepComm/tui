
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
  int fps = 60;
  IntervalP interval = Interval_create(fps);

  bool shouldLoop = true;

  SurfaceP canvas = Surface_create();
  char edit[100] = "hello world";
  int editSize = 4;

  int renderUpdates = 0;
  
  // continuous loop
  while (shouldLoop) {
    // keep track of time
    Timer_step(timer);

    // check if its time to render
    if (Interval_calculate(interval, timer)) {
      Terminal_clear();
      canvas->clear(canvas);

      // check if we need to update size
      if (Terminal_update_size()) {
        // printf ("Size changed to %d by %d\n", TERMINAL.width,
        // TERMINAL.height);
        Surface_setSize(canvas, (float)TERMINAL.width, (float)TERMINAL.height);
      }
      float w = (float)TERMINAL.width;
      float h = (float)TERMINAL.height;
      float min = w > h ? h : w;
      
      float clockSize = min / 2.0f; //3.0f;

      float handSize = clockSize * 0.8f;

      if (renderUpdates >= 60*fps) renderUpdates = 0;

      float percent = ((float) renderUpdates) / (float) fps / 60.0f;
      float angle = (-percent * 360) + 180;
      float rads = radians(angle);

      float x = w/2;
      float y = h/2;

      canvas->strokeChar = 'O';

      canvas->strokeLine(canvas,
        x, y,
        x + (handSize * sinf ( rads ) * FONT_RATIO),
        y + (handSize * cosf ( rads ))
      );

      canvas->strokeChar = '*';

      Surface_strokeCircle(canvas, x, y, clockSize, 512);


      // printf to canvas built in 'line' utility string variable
      float textWidth = (float)snprintf(canvas->line, canvas->lineLength,
                                        "Width : %d, Height : %d, Frame : %d",
                                        TERMINAL.width, TERMINAL.height, renderUpdates);

      renderUpdates++;
      // draw canvas->line in center of screen
      Surface_drawText(canvas, w / 2 - textWidth / 2, h / 2, canvas->line);

      

      if (editSize > 1) {
        // draw canvas->line in center of screen
        Surface_drawText(canvas, w / 2 - textWidth / 2, (h / 2) + 2.0f, edit);
      }

      // write buffer to terminal
      fwrite(canvas->buffer, canvas->buffersize - 1, 1, stdout);
      fflush(stdout);
    }
  }
  

}
