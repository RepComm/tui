
#include <stdbool.h>
#include <stdio.h>

#include "./terminal.c"
// #include "./tui.c"
#include "./3d/camera.c"
#include "./3d/mesh.c"
#include "./rect.c"
#include "./surface.c"
#include "./timer.c"

void Mesh_draw(CameraP camera, MeshP mesh, SurfaceP surface) {
  vec3 a, b, c;

  for (int i = 0; i < mesh->vertexCount; i += 9) {
    Camera_point_project(camera,
                         (vec3){mesh->verticies[i], mesh->verticies[i + 1],
                                mesh->verticies[i + 2]},
                         a);

    Camera_point_project(camera,
                         (vec3){mesh->verticies[i + 3], mesh->verticies[i + 4],
                                mesh->verticies[i + 5]},
                         b);

    Camera_point_project(camera,
                         (vec3){mesh->verticies[i + 6], mesh->verticies[i + 7],
                                mesh->verticies[i + 8]},
                         c);

    surface->strokeLine(surface, a[0], a[1], b[0], b[1]);
    surface->strokeLine(surface, b[0], b[1], c[0], c[1]);
    surface->strokeLine(surface, c[0], c[1], a[0], a[1]);
  }
}

int main(int argc, char **argv) {
  printf("Hello World");

  struct Camera cam;
  Camera_XYZ(&cam, 0.0, 0.0, 0.0);
  Camera_rotateEulerXYZ(&cam, 0.0, 0.0, 0.0);

  Camera_update(&cam);

  struct Mesh mesh;
  mesh.vertexCount = 9;
  mesh.verticies = (float[9]) {
    0.0f, 0.0f, 0.0f,
    1.0f, 1.0f, 1.0f,
    0.0f, 1.0f, 1.0f
  };

  TimerP timer = Timer_create();
  int fps = 5;
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

      float clockSize = min / 2.0f;  // 3.0f;

      float handSize = clockSize * 0.8f;

      if (renderUpdates >= 60 * fps) renderUpdates = 0;

      float percent = ((float)renderUpdates) / (float)fps / 60.0f;
      float angle = (-percent * 360) + 180;
      float rads = radians(angle);

      float x = w / 2;
      float y = h / 2;

      canvas->strokeChar = 'O';

      canvas->strokeLine(canvas, x, y, x + (handSize * sinf(rads) * FONT_RATIO),
                         y + (handSize * cosf(rads)));

      canvas->strokeChar = '*';

      Surface_strokeCircle(canvas, x, y, clockSize, 8);

      // printf to canvas built in 'line' utility string variable
      float textWidth =
          (float)snprintf(canvas->line, canvas->lineLength,
                          "Width : %d, Height : %d, Frame : %d", TERMINAL.width,
                          TERMINAL.height, renderUpdates);

      renderUpdates++;
      // draw canvas->line in center of screen
      Surface_drawText(canvas, w / 2 - textWidth / 2, h / 2, canvas->line);

      if (editSize > 1) {
        // draw canvas->line in center of screen
        Surface_drawText(canvas, w / 2 - textWidth / 2, (h / 2) + 2.0f, edit);
      }

      Mesh_draw(&cam, &mesh, canvas);

      // write buffer to terminal
      fwrite(canvas->buffer, canvas->buffersize - 1, 1, stdout);
      fflush(stdout);
    }
  }
}
