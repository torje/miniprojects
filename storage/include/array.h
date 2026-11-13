#include <stdbool.h>
#include <stddef.h>

size_t power_of_two_above(size_t n);

typedef struct Array {
  void *start;
  size_t length;
  size_t allocated;
  size_t size;
} Array;

Array array_init(size_t length, size_t size);
void array_deinit(Array *array);
bool array_reallocate(Array *array, size_t length);
void array_blind_push_back(Array *array, void *data);
bool array_push_back(Array *array, void *data);
void array_copy_back(Array *array, void *data);
void array_copy_index(Array *array, size_t index, void *data);
bool array_down_size(Array *array);
bool array_pop_back(Array *array);
