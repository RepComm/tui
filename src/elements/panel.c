
#ifndef ELEMENT_PANEL_C
#define ELEMENT_PANEL_C

#include "../element.c"

void Panel_render (ElementP thiz, char * buffer, int buffersize) {
  
}

ElementP Panel_create () {
  ElementP result = Element_create();
  result->renderCallback = &Panel_render;
  return result;
}

#endif
