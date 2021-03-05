
#ifndef MATH_C
#define MATH_C

#include <time.h>

const float m_f_epsilon = 0.00001;
const float m_f_pi = 3.14;
const float m_f_tau = 6.28;
const float m_f_piover2 = 1.5707963267948966;

float randomFloat () {
  return rand() / (float) RAND_MAX;
}

float m_sin_with_precision (float radians, int steps) {
  //keep radians between -pi and pi
  radians = fmod(radians, m_f_tau)-m_f_pi;

  //https://stackoverflow.com/a/2284929
  float res=0, pow=radians, fact=1;

  for(int i=0; i<steps; ++i) {
    res+=pow/fact;
    pow*=-1*radians*radians;
    fact*=(2*(i+1))*(2*(i+1)+1);
  }
  return res;
}

float m_get_seconds () {
  float res = 0;

  res = ((float) clock()) / CLOCKS_PER_SEC;

  return res;
}

float m_sin (float radians) {
  return m_sin_with_precision(radians, 10);
}

/**Linearly interpolate between `from` and `to`
 * 
 * From and To can be any floating point values
 * 
 * By is a float between 0.0 and 1.0
 * 
 * Given From = 0 and To = 12
 * By = 0.5 would return 6
 * By = 1.0 would return 12
 * By = 0.0 would return 0
*/
float lerp (float from, float to, float by) {
  return from*(1-by)+to*by;
}

/**In linear interpolation, we know From, To, and By (the interpolant)
 * 
 * In an inverseLerp, we know From, To, and the interpolated value
 * Inverse lerp will return the By (interpolant) used in a lerp operation
 * 
 * Given From, To, By
 * Value = lerp ( From, To, By )
 * 
 * Given From, To, Value
 * By = inverseLerp ( From, To, Value )
 */
float inverseLerp (float from, float to, float value) {
  //Fixes nan issue where to-from is a divide by zero
  if (to - from == 0.0) to += m_f_epsilon;
  return (value - from) / (to - from);
}

float m_arcsin (float x) {
  float x2 = x * x;
  float x3 = x2 * x;
  const float a = 1.5707288;
  const float b = -0.2121144;
  const float c = 0.0742610;
  const float d = -0.0187293;
  //TODO - implement sqrt
  return m_f_piover2 - sqrt(1 - x) * (a + b * x + c * x2 + d * x3);
}

/**Floating point absolute value
 */
float m_fabs (float f) {
  if (f < 0) return -f;
  return f;
}

void bline(void * meta, float x0, float y0, float x1, float y1, void (*drawAt)(void * meta, float drawx, float drawy)) {
  int maxIterations = 2048;
  int currentIteration = 0;

  float dx = m_fabs(x1 - x0);
  float sx = x0 < x1 ? 1 : -1;

  float dy = m_fabs(y1 - y0);
  float sy = y0 < y1 ? 1 : -1;

  float err = (dx > dy ? dx : -dy) / 2;
  float e2;

  while (1) {
    if (currentIteration > maxIterations) break;
    currentIteration ++;

    drawAt(meta, x0, y0);

    if ( m_fabs(x0 - x1) < m_f_epsilon && m_fabs(y0 - y1) < m_f_epsilon) break;
    e2 = err;
    if (e2 > -dx) {
      err -= dy;
      x0 += sx;
    }
    if (e2 < dy) {
      err += dx;
      y0 += sy;
    }
  }
}

void bline_ints (void * meta, float x0, float y0, float x1, float y1, void (*drawAt)(void * meta, int drawx, int drawy)) {
  int maxIterations = 2048;
  int currentIteration = 0;

  float dx = m_fabs(x1 - x0);
  float sx = x0 < x1 ? 1 : -1;

  float dy = m_fabs(y1 - y0);
  float sy = y0 < y1 ? 1 : -1;

  float err = (dx > dy ? dx : -dy) / 2;
  float e2;

  while (1) {
    if (currentIteration > maxIterations) break;
    currentIteration ++;

    drawAt(meta, (int) x0, (int) y0);

    if ( m_fabs(x0 - x1) < m_f_epsilon && m_fabs(y0 - y1) < m_f_epsilon) break;
    e2 = err;
    if (e2 > -dx) {
      err -= dy;
      x0 += sx;
    }
    if (e2 < dy) {
      err += dx;
      y0 += sy;
    }
  }
}

int m_2dToIndex (int x, int y, int width) {
  return x + width * y;
}
int m_IndexTo2dX(int index, int width) {
  return index % width;
}
int m_IndexTo2dY(int index, int width) {
  return index / width;
}

#endif