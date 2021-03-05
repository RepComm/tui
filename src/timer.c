
#ifndef TIMER_C
#define TIMER_C

#include <time.h>
#include <stdlib.h>

#include "./boolean.h"

#define TimerP struct Timer *
struct Timer {
  int iTargetFps;
  float iTargetInterval;
  float sLast;
  float sEnlapsed;
  float sNow;
  void (*step)(TimerP timer);
};

float getSeconds () {
  return (float)clock() / (float) CLOCKS_PER_SEC;
}

void Timer_step (TimerP timer) {
  timer->sEnlapsed = (timer->sNow - timer->sLast);
  timer->sLast = timer->sNow;
  timer->sNow = getSeconds();
}

TimerP Timer_create () {
  TimerP result = malloc(sizeof(struct Timer));
  result->sNow = getSeconds();
  result->sLast = result->sNow;
  result->sEnlapsed = result->sNow - result->sLast;
  result->step = &Timer_step;
  return result;
}

#define IntervalP struct Interval *
struct Interval {
  int targetFps;
  float targetEnlapsed;
  float enlapsed;
  bool (*shouldIterate)(IntervalP interval);
  void (*calculate)(IntervalP interval, TimerP timer);
};

void Interval_calculate (IntervalP interval, TimerP timer) {
  interval->enlapsed += timer->sEnlapsed;
}

bool Interval_shouldIterate (IntervalP interval) {
  if (interval->enlapsed >= interval->targetEnlapsed) {
    interval->enlapsed = 0;
    return true;
  }
  return false;
}

IntervalP Interval_create (int targetFps) {
  IntervalP result = malloc(sizeof(struct Interval));

  result->targetFps = targetFps;
  result->targetEnlapsed = 1.0 / (float) result->targetFps;
  result->shouldIterate = &Interval_shouldIterate;
  result->calculate = &Interval_calculate;

  return result;
}

#endif
