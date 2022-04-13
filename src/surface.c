
#ifndef SURFACE_C
#define SURFACE_C

#include <string.h>
#include <stdlib.h>
#include "./rect.c"
#include "./math.c"

#define SurfaceTranslateStackSize 100
#define SurfaceP struct Surface *
struct Surface {
  char * buffer;
  int buffersize;
  RectP rect;
  RectP rectPrevious;
  char strokeChar;
  void (*strokeLine)(SurfaceP surface, float x0, float y0, float x1, float y1);
  void (*strokeLineVecs)(SurfaceP surface, Vec2P from, Vec2P to);
  void (*strokePixel)(SurfaceP surface, int x, int y);
  void (*clear)(SurfaceP surface);
  /*
   * A utility variable
   * A string with enough memory to fit in the buffer as a single line
   */
  char * line;
  int lineLength;

  struct Vec2 translateStack[SurfaceTranslateStackSize];
  int translateStackSize;
  struct Vec2 translate;
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
  for (int i=0; i<surface->buffersize-1; i++) {
    surface->buffer[i] = ' ';
  }
  surface->buffer[surface->buffersize-1] = 0x00;
}

void Surface_strokeLine (SurfaceP surface, float x0, float y0, float x1, float y1) {
  bline_ints(surface, x0, y0, x1, y1, (void *) &Surface_strokePixel);
}

void Surface_strokeLineVecs (SurfaceP surface, Vec2P from, Vec2P to) {
  bline_ints(surface, from->x, from->y, to->x, to->y, (void *) &Surface_strokePixel);
}

void Surface_drawText (SurfaceP surface, float x, float y, char * text) {
  int textLength = strlen(text);
  int idx = 0;

  for (int i=0; i<textLength; i++) {
    idx = Surface_isValidPixel(surface, x+i, y);
    if (idx == -1) break;

    //TODO - check if necessary, probably is in some terminals
    if (text[i] == '\n') continue;

    surface->buffer[idx] = text[i];
  }
}

void Surface_setSize(SurfaceP surface, float w, float h) {
  Rect_copy(surface->rectPrevious, surface->rect);
  
  surface->rect->size->set(surface->rect->size, w, h);

  surface->buffersize = (int) w * (int) h + 1;
  if (surface->buffer == NULL) {
    free(surface->buffer);
  }
  surface->buffer = (char *) malloc(sizeof(char) * surface->buffersize);
  
  if (surface->line != NULL) free(surface->line);
  surface->lineLength = (int) w;
  surface->line = (char *) malloc(sizeof(char) * surface->lineLength);

  Surface_clear(surface);
}

SurfaceP Surface_create () {
  SurfaceP result = malloc(sizeof(struct Surface));

  result->rectPrevious = Rect_create();
  result->rect = Rect_create();

  result->buffer = NULL;
  result->buffersize = 0;
  Surface_setSize(result, 32.0, 32.0);

  result->strokeLine = &Surface_strokeLine;
  result->strokeLineVecs = &Surface_strokeLineVecs;
  result->strokeChar = '$';
  result->strokePixel = &Surface_strokePixel;

  result->clear = &Surface_clear;

  result->translateStackSize = 0;
  Vec2_set(&result->translate, 0.0, 0.0);
  return result;
}

bool Surface_destroy (SurfaceP surface) {
  if (surface == NULL) return false;
  free(surface->buffer);
  
  Rect_destroy( surface->rect );
  Rect_destroy( surface->rectPrevious );
  
  if (surface->line != NULL) free(surface->line);

  free(surface);
  return true;
}

bool Surface_translatePush (SurfaceP surface) {
  if (surface->translateStackSize+1 > SurfaceTranslateStackSize) return false;
  Vec2_copy(
    &surface->translateStack[surface->translateStackSize],
    &surface->translate
  );
  surface->translateStackSize++;
  return true;
}
bool Surface_translatePop (SurfaceP surface) {
  if (surface->translateStackSize < 1) return false;

  Vec2_copy(
    &surface->translate,
    &surface->translateStack[surface->translateStackSize]
  );
  surface->translateStackSize--;

  return true;
}

void Surface_translate (SurfaceP surface, int x, int y) {
  surface->translate.x += x;
  surface->translate.y += y;
}

void Surface_translateByRect (SurfaceP surface, RectP rect) {
  Surface_translate(surface, (int)rect->position->x, (int)rect->position->y);
}

void Surface_confine (SurfaceP surface, RectP rect) {

}

bool Surface_isConfined (SurfaceP surface, int x, int y) {
  return true;
}
bool Surface_isHeightConfined (SurfaceP surface, int y) {
  return true;
}

#endif
