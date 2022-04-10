
#ifndef LINKEDLIST_C
#define LINKEDLIST_C

#include <stdbool.h>

struct lln {
  void * value;
  //the first node in the list (may be the same as current)
  struct lln * start;
  //the next node in the list
  struct lln * next;
};

//Typing "struct lln *" is annoying
#define llnp struct lln *

llnp lln_create () {
  llnp link = malloc(sizeof(struct lln));
  link->start = link;
  link->next = 0;
  link->value = 0;
  return link;
}

void lln_add (llnp parent, llnp child) {
  parent->next = child;
  child->start = parent->start;
}

llnp lln_get_tail (llnp parent) {
  llnp current = parent;
  while (current->next != 0) {
    current = current->next;
  }
  return current;
}

llnp lln_add_value (llnp parent, void * value) {
  llnp node = lln_create();
  node->value = value;

  lln_add(lln_get_tail(parent), node);

  return node;
}

bool lln_has (llnp list, llnp search) {
  llnp current = list->start;

  while (current != 0) {
    if (current == search) return true;
    current = current->next;
  }
  return false;
}

bool lln_has_value (llnp list, void * value) {
  llnp current = list->start;

  while (current != 0) {
    if (current->value == value) return true;
    current = current->next;
  }
  return false;
}

void lln_destroy (llnp list) {
  llnp current = lln_get_tail(list);

  while (current != 0) {
    free(current);
    current = lln_get_tail(list);
  }
}

/**Get the number of items in the linked list
 * If linked list is null/0 this will return 0
 * Otherwise it will loop through the list, returning the count
 */
int lln_get_size (llnp list) {
  if (list == 0) return 0;
  
  llnp current = list->start;
  int counter = 0;
  while (current != 0) {
    counter ++;
    current = current->next;
  }
  return counter;
}

/**Iterate through the linked list with a callback function
 * node - a linked list node (usually the start aka tail)
 */
int lln_iterate ( llnp node, void (*cb)() ) {
  llnp cn = node->start;

  int counter = 0;

  while (cn != 0x0) {
    (*cb)();
    counter++;
    cn = cn->next;
  }
  return counter;
}

#endif
