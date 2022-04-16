
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

    if (a[2] > 0.0f && b[2] > 0.0f) surface->strokeLine(surface, a[0], a[1], b[0], b[1]);
    if (c[2] > 0.0f && b[2] > 0.0f) surface->strokeLine(surface, b[0], b[1], c[0], c[1]);
    if (a[2] > 0.0f && c[2] > 0.0f) surface->strokeLine(surface, c[0], c[1], a[0], a[1]);
  }
}

int main(int argc, char **argv) {
  printf("Hello World\n");

  struct Camera camera;
  Camera_setAspect(&camera, 1.0f);
  Camera_setFieldOfView(&camera, 70.0f);
  Camera_setNearFar(&camera, 0.01f, 100.0f);
  Camera_setViewPort(&camera, 0.0f, 0.0f, 50.0f, 50.0f);

  printf("viewport %f %f %f %f\n", camera.viewPort[0], camera.viewPort[1],
         camera.viewPort[2], camera.viewPort[3]);

  // Camera_lookat(&camera, (vec3){3.0f, 2.0f, 0.0f});

  float size = 20.0f;
  float tileSize = 5.0f;

  int idx = 0;
  for (float x = 0.0f; x < size; x += tileSize) {
    for (float y = 0.0f; y < size; y += tileSize) {
      idx++;
      idx++;
      idx++;

      idx++;
      idx++;
      idx++;

      idx++;
      idx++;
      idx++;

      idx++;
      idx++;
      idx++;

      idx++;
      idx++;
      idx++;

      idx++;
      idx++;
      idx++;
    }
  }

  struct Mesh mesh;
  int vertexCount = idx;
  printf("allocating %d verts\n", vertexCount);
  fflush(stdout);

  mesh.verticies = (float *)malloc(sizeof(float) * vertexCount);
  mesh.vertexCount = vertexCount;

  float halfSize = size/2.0f;
  idx = 0;
  for (float x = -halfSize; x < halfSize; x += tileSize) {
    for (float y = -halfSize; y < halfSize; y += tileSize) {
      // triangle a
      /*
      a----b
      |   /
      | /
      c
      */
      mesh.verticies[idx] = x;
      idx++;
      mesh.verticies[idx] = 0.0f;
      idx++;
      mesh.verticies[idx] = y;
      idx++;

      mesh.verticies[idx] = x + tileSize;
      idx++;
      mesh.verticies[idx] = 0.0f;
      idx++;
      mesh.verticies[idx] = y;
      idx++;

      mesh.verticies[idx] = x;
      idx++;
      mesh.verticies[idx] = 0.0f;
      idx++;
      mesh.verticies[idx] = y + tileSize;
      idx++;

      // triangle b
      /*
           a
          /|
        /  |
      c----b
      */
      mesh.verticies[idx] = x + tileSize;
      idx++;
      mesh.verticies[idx] = 0.0f;
      idx++;
      mesh.verticies[idx] = y;
      idx++;

      mesh.verticies[idx] = x + tileSize;
      idx++;
      mesh.verticies[idx] = 0.0f;
      idx++;
      mesh.verticies[idx] = y + tileSize;
      idx++;

      mesh.verticies[idx] = x;
      idx++;
      mesh.verticies[idx] = 0.0f;
      idx++;
      mesh.verticies[idx] = y + tileSize;
      idx++;
    }
  }

  Transform_update(&mesh.transform);

  TimerP timer = Timer_create();
  int fps = 5;
  IntervalP interval = Interval_create(fps);

  bool shouldLoop = true;

  SurfaceP canvas = Surface_create();
  char edit[100] = "hello world";
  int editSize = 4;

  int renderUpdates = 0;

  float adjust = 0.0f;

  // continuous loop
  while (shouldLoop) {
    // keep track of time
    Timer_step(timer);

    // check if its time to render
    if (Interval_calculate(interval, timer)) {
      Terminal_clear();
      canvas->clear(canvas);

      adjust += 0.1f;
      camera.transform.position[1] = adjust;
      Camera_update(&camera, mesh.transform.modelMatrix, true, true);

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

      // canvas->strokeChar = 'O';

      // canvas->strokeLine(canvas, x, y, x + (handSize * sinf(rads) *
      // FONT_RATIO),
      //                    y + (handSize * cosf(rads)));

      // canvas->strokeChar = '*';

      // Surface_strokeCircle(canvas, x, y, clockSize, 8);

      // printf to canvas built in 'line' utility string variable
      float textWidth =
          (float)snprintf(canvas->line, canvas->lineLength,
                          "Width : %d, Height : %d, Frame : %d", TERMINAL.width,
                          TERMINAL.height, renderUpdates);

      renderUpdates++;
      // draw canvas->line in center of screen
      // Surface_drawText(canvas, w / 2 - textWidth / 2, h / 2, canvas->line);

      // if (editSize > 1) {
      //   // draw canvas->line in center of screen
      //   Surface_drawText(canvas, w / 2 - textWidth / 2, (h / 2) + 2.0f,
      //   edit);
      // }

      canvas->strokeChar = '#';
      Mesh_draw(&camera, &mesh, canvas);

      // write buffer to terminal
      fwrite(canvas->buffer, canvas->buffersize - 1, 1, stdout);
      fflush(stdout);
    }
  }
}
