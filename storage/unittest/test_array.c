#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <stdio.h>

#include "include/array.h"

void test_array_initialization_deinitialization(void) {
  Array array_int = array_init(0, sizeof(int));
  CU_ASSERT_PTR_NOT_NULL(array_int.start);
  CU_ASSERT_EQUAL(array_int.allocated, 1);
  CU_ASSERT_EQUAL(array_int.length, 0);
  array_deinit(&array_int);
  CU_ASSERT_PTR_NULL(array_int.start);
  CU_ASSERT_EQUAL(array_int.allocated, 0);
  CU_ASSERT_EQUAL(array_int.length, 0);
}

void test_push_pop(void) {
  Array array_int = array_init(0, sizeof(int));
  for (int i = 0; i < 1 << 20; i++) {
    int num = 22;
    array_push_back(&array_int, &num);
  }
  CU_ASSERT(1ull << 20 == array_int.length);
  CU_ASSERT(array_int.length <= array_int.allocated);
  for (int i = 0; i < 1 << 20; i++) {
    array_pop_back(&array_int);
  }
  CU_ASSERT(0 == array_int.length);
  array_deinit(&array_int);
}
void test_array_copy(void) {
  Array array_int = array_init(0, sizeof(int));
  for (int i = 0; i < 16; i++) {
    array_push_back(&array_int, &i);
  }
  int data;
  array_copy_back(&array_int, &data);
  CU_ASSERT_EQUAL(data, 15);
  array_copy_index(&array_int, 5, &data);
  CU_ASSERT_EQUAL(data, 5);
  array_pop_back(&array_int);
  array_pop_back(&array_int);
  array_pop_back(&array_int);
  array_pop_back(&array_int);
  array_pop_back(&array_int);
  array_copy_back(&array_int, &data);
  CU_ASSERT_EQUAL(data, 10);

  array_deinit(&array_int);
}

int main(int argc, const char *const *const argv) {

  /* initialize the CUnit test registry */
  if (CUE_SUCCESS != CU_initialize_registry())
    return CU_get_error();

  CU_pSuite suite = CU_add_suite("array_testsuite", NULL, NULL);
  if (suite == NULL) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  void *array_init_test =
      CU_add_test(suite, "test_array_initialization_deinitialization",
                  test_array_initialization_deinitialization);
  void *array_push_pop =
      CU_add_test(suite, "test_array_push_pop", test_push_pop);
  void *array_copy_element =
      CU_add_test(suite, "copy element", test_array_copy);
  if (!(array_init_test && array_push_pop && array_copy_element)) {
    printf("Failed to initialize test");
    CU_cleanup_registry();
    return CU_get_error();
  }

  CU_basic_set_mode(CU_BRM_VERBOSE);
  CU_basic_run_tests();
  CU_cleanup_registry();
  return CU_get_error();
}
