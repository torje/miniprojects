#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/array.h"

size_t power_of_two_above(size_t n) {

  if (n == 0) {
    return 1;
  }
  size_t lz = __builtin_clzl(n);
  size_t shift = sizeof(n) * 8 - lz;
  size_t allocation_size = 1ull << shift;
  return allocation_size;
}

Array array_init(size_t length, size_t size) {
  size_t allocation_size = power_of_two_above(length);
  void *start = calloc(allocation_size, size);
  Array array = {.start = start,
                 .length = length,
                 .allocated = allocation_size,
                 .size = size};
  return array;
}

void array_deinit(Array *array) {
  free(array->start);
  array->start = NULL;
  array->length = 0;
  array->size = 0;
  array->allocated = 0;
}
void *array_index_location(Array *array, size_t index) {
  return array->start + array->size * index;
}
size_t array_index_offset(Array *array, size_t index) {
  return array->size * index;
}
bool array_reallocate(Array *array, size_t length) {
  size_t allocation_size = power_of_two_above(length);
  void *new_start = reallocarray(
      array->start, array_index_offset(array, length) * array->size,
      array->size);
  if (new_start) {
    array->start = new_start;
    array->allocated = allocation_size;
    return true;
  }
  return false;
}
void array_blind_push_back(Array *array, void *data) {
  memmove(array_index_location(array, array->length), data, array->size);
  array->length++;
}
void print_array(Array *array) {
  printf("{\n"
         "  start: %p\n"
         "  length: %zu \n"
         "  size: %zu\n"
         "  allocated: %zu\n"
         "}\n",
         array->start, array->length, array->size, array->allocated);
}
bool array_push_back(Array *array, void *data) {
  if (array->length < array->allocated) {
    array_blind_push_back(array, data);
    return true;
  } else {
    bool success = array_reallocate(array, array->length + 1);
    if (success) {
      array_blind_push_back(array, data);
      return true;
    } else {
      return false;
    }
  }
  return false;
}
void array_copy_back(Array *array, void *data) {
  memcpy(data, array_index_location(array, array->length - 1), array->size);
}
void array_copy_index(Array *array, size_t index, void *data) {
  memcpy(data, array_index_location(array, index), array->size);
}

bool array_down_size(Array *array) {
  if (array->length * 4 < array->allocated) {
    size_t new_allocation_size = power_of_two_above(array->length);
    void *new_start =
        reallocarray(array->start, new_allocation_size, array->size);
    if (new_start) {
      array->start = new_start;
      array->allocated = new_allocation_size;
      return true;
    } else
      return false;
  }
  return false;
}

bool array_pop_back(Array *array) {
  array->length--;
  return array_down_size(array);
}
