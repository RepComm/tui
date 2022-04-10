
#ifndef VEC2_C
#define VEC2_C

#include "./math.c"
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

#define Vec2P struct Vec2 *
struct Vec2 {
  float x;
  float y;
  void (*set)(Vec2P thiz, float x, float y);
};

void Vec2_set (Vec2P thiz, float x, float y) {
  thiz->x = x;
  thiz->y = y;
}

bool Vec2_equals (Vec2P first, Vec2P second) {
  return(
    m_fabs(first->x - second->x) < m_f_epsilon &&
    m_fabs(first->y - second->y) < m_f_epsilon
  );
}

float Vec2_dist (Vec2P first, Vec2P second) {
  float dx = first->x - second->x;
  dx *= dx;

  float dy = first->y - second->y;
  dy *= dy;

  return sqrt(
    dx + dy
  );
}

void Vec2_add (Vec2P to, Vec2P from) {
  to->x += from->x;
  to->y += from->y;
}

void Vec2_sub (Vec2P to, Vec2P from) {
  to->x -= from->x;
  to->y -= from->y;
}

void Vec2_mul (Vec2P to, Vec2P from) {
  to->x *= from->x;
  to->y *= from->y;
}

float Vec2_mag (Vec2P vec) {
  float x2 = vec->x;
  x2 *= x2;
  float y2 = vec->y;
  y2 *= y2;
  return sqrtf (x2 + y2);
}

void Vec2_copy (Vec2P to, Vec2P from) {
  to->x = from->x;
  to->y = from->y;
}

Vec2P Vec2_create () {
  Vec2P result = malloc(sizeof(struct Vec2));
  result->x = 0;
  result->y = 0;
  result->set = &Vec2_set;
  return result;
}

bool Vec2_destroy (Vec2P vec) {
  if (vec == NULL) return false;
  free(vec);
  return true;
}

#endif
