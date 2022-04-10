
#ifndef RECT_C
#define RECT_C

#include "./vec2.c"
#include <stdlib.h>

#define RectP struct Rect *
struct Rect {
  Vec2P position;
  Vec2P size;
};

void Rect_copy (RectP to, RectP from) {
  Vec2_copy(to->position, from->position);
  Vec2_copy(to->size, from->size);
}

RectP Rect_create () {
  RectP result = malloc(sizeof(struct Rect));
  result->position = Vec2_create();
  result->size = Vec2_create();
  return result;
}

bool Rect_destroy (RectP rect) {
  if (rect == NULL) return false;
  Vec2_destroy(rect->position);
  Vec2_destroy(rect->size);
  free(rect);
  return true;
}

#endif
