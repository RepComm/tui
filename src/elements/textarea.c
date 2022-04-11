
#ifndef TEXTAREA_C
#define TEXTAREA_C

#include <stdio.h>
#include <stdbool.h>
#include "../linkedlist.c"
#include "../surface.c"
#include "../rect.c"

#define TextAreaP struct TextArea *
struct TextArea {
  llnp lines;
  llnp currentLine;

  int lineCount;
  bool focus;

  int scrollX;
  int scrollY;
  RectP rect;
};

llnp TextArea_getLine(TextAreaP ta, int offset) {
  return lln_get_at(ta->lines, offset);
}

TextAreaP TextArea_create () {
  TextAreaP result = (TextAreaP) malloc(sizeof(struct TextArea));

  result->lines = NULL;

  result->lineCount = 0;

  return result;
}

llnp TextArea_lineInsertAfter (llnp parent) {
  llnp result = lln_create();
  
  result->next = parent->next;
  lln_add(parent, result);

  return result;
}

/**
 * WARNING: Traverses all lines in linked list, prefer TextArea_lineInsertAfter when possible, and use caching
 */
llnp TextArea_lineInsertAt (llnp node, int offset) {
  llnp at = lln_get_at(node, offset);
  return TextArea_lineInsertAfter(at);
}

void TextArea_draw (TextAreaP ta, SurfaceP surface) {
  //save what was going on previously
  Surface_translatePush(surface);

  //move to where our textarea is located
  Surface_translateByRect(surface, ta->rect);

  //restrict drawing to this area
  Surface_confine(surface, ta->rect);

  //scroll position
  Surface_translate(surface, - ta->scrollX, - ta->scrollY);

  llnp line = ta->lines;
  int lineIndex = 0;

  //don't draw lines we don't have
  while (line != 0x00) {

    //don't draw lines hidden by scroll Y
    if (lineIndex < ta->scrollY) continue;

    //don't draw lines not confined to drawing rect heights (optimisation)
    if (!Surface_isHeightConfined(
      surface, lineIndex
    )) continue;

    //render line data when available (hopefully always?)
    if (line->value) {
      Surface_drawText(
        surface, 0, 0,
        line->value
      );
    }
    
    line = line->next;
    lineIndex ++;
  }

  //undo our translations
  Surface_translatePop(surface);
}

#endif
