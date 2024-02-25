#include <check.h>
#include <stdlib.h>
#include <string.h>

#define ARENA_H_IMPLEMENTATION
#include "arena.h"

START_TEST(arena_calloc_ok) {
  size_t arena_size = (1 << 16) - 1;
  void *arena_ptr = malloc(arena_size);
  memset(arena_ptr, 1, arena_size);

  Arena arena = arena_init(arena_ptr, arena_size);

  char *c = arena_calloc(&arena, 1, 1);
  ck_assert_ptr_nonnull(c);
  ck_assert_uint_ge((uintptr_t)c, (uintptr_t)arena_ptr);
  ck_assert_uint_lt((uintptr_t)c, (uintptr_t)arena_ptr + arena_size);
  ck_assert_uint_eq(*c, 0);
}
END_TEST

START_TEST(arena_calloc_ok_size_zero) {
  size_t arena_size = (1 << 16) - 1;
  void *arena_ptr = malloc(arena_size);
  Arena arena = arena_init(arena_ptr, arena_size);

  size_t *ptr = arena_calloc(&arena, 0, 0);
  ck_assert_ptr_nonnull(ptr);

  size_t *ptr2 = arena_calloc(&arena, 0, 0);
  ck_assert_ptr_nonnull(ptr);
  ck_assert_ptr_ne(ptr, ptr2);
}
END_TEST

START_TEST(arena_calloc_ok_arena_size) {
  size_t arena_size = (1 << 16) - 1;
  void *arena_ptr = malloc(arena_size);
  Arena arena = arena_init(arena_ptr, arena_size);

  size_t *ptr = arena_calloc(&arena, 1, arena_size);
  ck_assert_ptr_nonnull(ptr);
}
END_TEST

START_TEST(arena_calloc_fail) {
  size_t arena_size = (1 << 16) - 1;
  void *arena_ptr = malloc(arena_size);
  Arena arena = arena_init(arena_ptr, arena_size);

  size_t *ptr = arena_calloc(&arena, 2, arena_size);
  ck_assert_ptr_null(ptr);
}
END_TEST

START_TEST(arena_calloc_fail_overflow) {
  size_t arena_size = (1 << 16) - 1;
  void *arena_ptr = malloc(arena_size);
  Arena arena = arena_init(arena_ptr, arena_size);

  size_t *ptr = arena_calloc(&arena, SIZE_MAX, SIZE_MAX);
  ck_assert_ptr_null(ptr);
}
END_TEST

static Suite *arena_calloc_suite(void) {
  Suite *s = suite_create("arena_alloc");
  TCase *tc_core = tcase_create("Core");

  tcase_add_test(tc_core, arena_calloc_ok);
  tcase_add_test(tc_core, arena_calloc_ok_size_zero);
  tcase_add_test(tc_core, arena_calloc_ok_arena_size);
  tcase_add_test(tc_core, arena_calloc_fail);
  tcase_add_test(tc_core, arena_calloc_fail_overflow);
  suite_add_tcase(s, tc_core);

  return s;
}

int main(void) {
  Suite *suite = arena_calloc_suite();
  SRunner *sr = srunner_create(suite);

  srunner_run_all(sr, CK_NORMAL);
  int number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  return number_failed == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
