#include <check.h>
#include <stdint.h>
#include <stdlib.h>

#define ARENA_H_IMPLEMENTATION
#include "arena.h"

START_TEST(arena_alloc_ok) {
  size_t arena_size = (1 << 16) - 1;
  void *arena_ptr = malloc(arena_size);
  Arena arena = arena_init(arena_ptr, arena_size);

  size_t *ptr = arena_alloc(&arena, sizeof(size_t));
  ck_assert_ptr_nonnull(ptr);
  ck_assert_uint_ge((uintptr_t)ptr, (uintptr_t)arena_ptr);
  ck_assert_uint_lt((uintptr_t)ptr, (uintptr_t)arena_ptr + arena_size);
}
END_TEST

START_TEST(arena_alloc_ok_size_zero) {
  size_t arena_size = (1 << 16) - 1;
  void *arena_ptr = malloc(arena_size);
  Arena arena = arena_init(arena_ptr, arena_size);

  size_t *ptr = arena_alloc(&arena, 0);
  ck_assert_ptr_nonnull(ptr);

  size_t *ptr2 = arena_alloc(&arena, 0);
  ck_assert_ptr_nonnull(ptr);
  ck_assert_ptr_ne(ptr, ptr2);
}
END_TEST

START_TEST(arena_alloc_ok_arena_size) {
  size_t arena_size = (1 << 16) - 1;
  void *arena_ptr = malloc(arena_size);
  Arena arena = arena_init(arena_ptr, arena_size);

  size_t *ptr = arena_alloc(&arena, arena_size);
  ck_assert_ptr_nonnull(ptr);
}
END_TEST

START_TEST(arena_alloc_fail) {
  size_t arena_size = (1 << 16) - 1;
  void *arena_ptr = malloc(arena_size);
  Arena arena = arena_init(arena_ptr, arena_size);

  size_t *ptr = arena_alloc(&arena, arena_size + 1);
  ck_assert_ptr_null(ptr);
}
END_TEST

static Suite *arena_alloc_suite(void) {
  Suite *s = suite_create("arena_alloc");
  TCase *tc_core = tcase_create("Core");

  tcase_add_test(tc_core, arena_alloc_ok);
  tcase_add_test(tc_core, arena_alloc_ok_size_zero);
  tcase_add_test(tc_core, arena_alloc_ok_arena_size);
  tcase_add_test(tc_core, arena_alloc_fail);
  suite_add_tcase(s, tc_core);

  return s;
}

int main(void) {
  Suite *suite = arena_alloc_suite();
  SRunner *sr = srunner_create(suite);

  srunner_run_all(sr, CK_NORMAL);
  int number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  return number_failed == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
