
#ifndef SURFACE_C
#define SURFACE_C

#include <stdlib.h>
#include "./rect.c"
#include "./math.c"

#define SurfaceP struct Surface *
struct Surface {
  char * buffer;
  int buffersize;
  RectP rect;
  char strokeChar;
  void (*strokeLine)(SurfaceP surface, Vec2P from, Vec2P to);
  void (*strokePixel)(SurfaceP surface, int x, int y);
  void (*clear)(SurfaceP surface);
};

int Surface_pixelToIdx (SurfaceP surface, int x, int y) {
  //TODO int cast is probably pretty slow
  float width = surface->rect->size->x;
  int intWidth = (int) width;

  // printf("converted float %f to int %i\n", width, intWidth);
  return m_2dToIndex (x, y, intWidth);
}

bool Surface_isValidBufferIndex (SurfaceP surface, int idx) {
  return idx > -1 && idx < surface->buffersize;
}

/**Checks if a pixel is a valid screen buffer pixel
 * If not, returns -1, otherwise, returns the buffer index
 */
int Surface_isValidPixel (SurfaceP surface, int x, int y) {
  if (x < 0 || y < 0) return -1;
  int idx = Surface_pixelToIdx(surface, x, y);

  if (Surface_isValidBufferIndex(surface, idx)) return idx;
  return -1;
}

void Surface_strokePixel (SurfaceP surface, int x, int y) {
  int idx = Surface_isValidPixel(surface, x, y);
  if (idx == -1) return;

  if (surface->buffer == 0) {
    printf("screen buffer is null\n");
    return;
  } else {
    // printf("stroked pixel at %i, %i aka buffer[%i]\n", x, y, idx);
  }

  surface->buffer[idx] = surface->strokeChar;
}

void Surface_clear (SurfaceP surface) {
  for (int i=0; i<surface->buffersize; i++) {
    surface->buffer[i] = ' ';    
  }
}

void Surface_strokeLine (SurfaceP surface, Vec2P from, Vec2P to) {
  bline_ints(surface, from->x, from->y, to->x, to->y, (void *) &Surface_strokePixel);
}

/**Surface_destroy is not responsible for rect or buffer!
 * Whatever code is using Surface_create needs to also Surface_destroy, and free rect and buffer
 */
SurfaceP Surface_create (RectP rect, char * buffer, int buffersize) {
  SurfaceP result = malloc(sizeof(struct Surface));

  result->rect = rect;
  result->strokeLine = &Surface_strokeLine;
  result->strokeChar = '$';
  result->strokePixel = &Surface_strokePixel;
  result->buffer = buffer;
  result->buffersize = buffersize;
  result->clear = &Surface_clear;

  return result;
}

bool Surface_destroy (SurfaceP surface) {
  if (surface == 0) return false;
  free(surface);
  return true;
}

#endif
