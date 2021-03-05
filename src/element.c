
#ifndef ELEMENT_C
#define ELEMENT_C

#include "./boolean.h"
#include "./linkedlist.c"
#include "./rect.c"
#include "./surface.c"

#define ElementP struct Element *
struct Element {
  RectP rect;
  llnp children;
  /**The function that actually performs rendering, passed per instance*/
  void (*renderCallback)(ElementP thiz, SurfaceP surface);
  /**Member function, same as Element_render, will end up calling renderCallback*/
  bool (*render)(ElementP thiz, SurfaceP surface);
  bool (*renderChildren)(ElementP thiz, SurfaceP surface);
};

/**Adds child to the parent
 */
void Element_add (ElementP parent, ElementP child) {
  if (parent->children == 0) {
    //Create linked list
    parent->children = lln_create();
    parent->children->value = child;
  } else {
    //Append to linked list
    lln_add_value(parent->children, child);
  }
}

/**Test if parent has child
 */
bool Element_has (ElementP parent, ElementP child) {
  return lln_has_value(parent->children, child);
}

bool Element_render (ElementP element, SurfaceP surface) {
  if (element == 0) return false;
  if (element->renderCallback == 0) return false;
  element->renderCallback(element, surface);
  return true;
}

/**Call render on all children of element*/
bool Element_renderChildren (ElementP element, SurfaceP surface) {
  if (element->children == 0) return false;
  llnp current = element->children;
  ElementP currentElement;

  while (current != 0) {
    currentElement = current->value;
    if (currentElement != 0) currentElement->render(currentElement, surface);

    current = current->next;
  }
  return true;
}

/**Create an element
 */
ElementP Element_create () {
  ElementP result = malloc(sizeof(struct Element));

  result->rect = Rect_create();
  result->children = 0;
  result->renderCallback = 0;

  result->render = &Element_render;
  result->renderChildren = &Element_renderChildren;

  return result;
}

bool Element_destroy (ElementP element) {
  if (element == 0) return false;
  Rect_destroy(element->rect);
  //TODO - protocol for children being de-parented?
  lln_destroy(element->children);
  free(element);
  return true;
}

#endif
