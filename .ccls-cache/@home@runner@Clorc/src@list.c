#include "../include/utils/list.h" 
#include <stdlib.h>
#include <stdio.h>
list_T *init_list() {
  list_T* list = (list_T*)calloc(1, sizeof(list_T));
  list->items = 0;
  list->used = 0;
  return list;
}
void add_element_list (list_T* list, void *item) {
  if (!list) {
    printf("Null oid");
    int* p = NULL;
    *p = 5;
  }
  list->used++;
  if (!list->items) {
    list->items = calloc(1, sizeof(void*));
  } else {
    list->items = realloc(list->items, sizeof(void*) * list->used);
  }
  list->items[list->used - 1] = item;
}

void free_list(list_T* list) {
  if(!list) return;
  if (list->used != 0) free(list->items);
  free(list);
}