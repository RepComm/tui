
#ifndef EDIT_C
#define EDIT_C

struct Edit {
  int maxSize;
  char * data;
  int offset;
};
#define EditP struct Edit *
EditP Edit_create () {
  int maxSize = 64;
  EditP result = (EditP) malloc(sizeof(struct Edit));
  result->maxSize = maxSize;
  result->data = (char *) malloc(sizeof(char) * result->maxSize);
  result->offset = 0;
  return result;
}

void Edit_destroy (EditP edit) {
  free(edit->data);
  free(edit);
}

void Edit_append (EditP edit, char c) {
  if (c == '\b' && edit->offset > 0) {
    edit->data[edit->offset] = 0;
    edit->offset --;
  } else {
    edit->data[edit->offset] = c;
    edit->offset ++;
  }
}

#endif
