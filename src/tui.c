
#ifndef TUI_C
#define TUI_C

#include <stdlib.h>

#include "./linkedlist.c"
#include "./rect.c"
#include "./element.c"

#define TUIDataP struct TUIData *
struct TUIData {
  /**Rendering library*/
  SurfaceP surface;

  RectP rect;
  /**A linked list of all the elements*/
  llnp elements;
  /**The root element*/
  ElementP root;
  
  /**The textual render target*/
  char * screenBuffer;
  /**Includes +1 for the null terminator*/
  int screenBufferAllocSize;
  /**screen buffer size without the null terminator*/
  int screenBufferContentSize;

  void (*render)(TUIDataP tui);
  void (*setSize)(TUIDataP tui, float width, float height);
  void (*clear)();
};

void TUI_Render (TUIDataP tui) {
  tui->clear();
  tui->surface->clear(tui->surface);

  if (tui->root) {
    tui->root->render(tui->root, tui->surface);
  } else {
    //TODO - handle no root element
  }
}

void TUI_setSize (TUIDataP tui, float width, float height) {
  printf("[TUI] Set size %i by %i\n", (int) width, (int) height);
  Vec2P size = tui->rect->size;
  size->set(size, width, height);

  //recalculate screen buffer size/allocate
  if (tui->screenBuffer != NULL) free(tui->screenBuffer);

  tui->screenBufferContentSize = ((int) (width * height));
  //+1 for null terminator
  tui->screenBufferAllocSize = tui->screenBufferContentSize + 1;

  tui->screenBuffer = malloc(tui->screenBufferAllocSize);
  //explicit null terminator
  tui->screenBuffer[tui->screenBufferAllocSize -1] = 0;

  // printf("[TUI] Created screen buffer[%i]\n", tui->screenBufferAllocSize);

  if (tui->surface == NULL) {
    tui->surface = Surface_create(
      tui->rect,
      tui->screenBuffer,
      tui->screenBufferContentSize
    );
  }
}

TUIDataP TUI_create (void (*clear)()) {
  TUIDataP result = malloc(sizeof(struct TUIData));
  
  result->elements = 0;
  result->rect = Rect_create();

  result->render = &TUI_Render;
  result->setSize = &TUI_setSize;
  result->clear = clear;
  return result;
}

#endif
