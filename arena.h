#ifndef ARENA_H_INCLUDE
#define ARENA_H_INCLUDE

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#ifndef ARENA_ASSERT
#include <assert.h>
#define ARENA_ASSERT assert
#endif

typedef struct {
  uintptr_t data;
  size_t size;
  size_t cursor;
} Arena;

/* The arena_init() function initialized a new arena with the given arena of
 * size bytes and returns it. */
Arena arena_init(void *arena, size_t size);

#ifdef ARENA_H_IMPLEMENTATION
Arena arena_init(void *data, size_t size) {
  Arena arena = {
      .size = size,
      .data = (uintptr_t)data,
      .cursor = 0,
  };

  return arena;
}
#endif

/* The arena_new() function allocates a new arena of size bytes and returns it.
 * Returned arena can then be passed to arena_free() to free ressources. */
Arena arena_new(size_t size);

#ifdef ARENA_H_IMPLEMENTATION
Arena arena_new(size_t size) {
  void *data = malloc(size);
  Arena arena = arena_init(data, size);
  return arena;
}
#endif

/* The arena_alloc() function allocates size bytes and returns a pointer to the
 * allocated memory. The memory is not initialized. If size is 0, then malloc()
 * returns a unique pointer value. */
void *arena_alloc(Arena *arena, size_t size);

#ifdef ARENA_H_IMPLEMENTATION
void *arena_alloc(Arena *arena, size_t size) {
  if (size > arena->size) {
    return NULL;
  }

  if (arena->size > SIZE_MAX - size) {
    return NULL;
  }

  uintptr_t ptr = arena->data + arena->cursor;
  arena->cursor += size > 0 ? size : 1;

  return (void *)ptr;
}
#endif

/* The arena_calloc() function allocates memory for an array of nmemb elements
 * of size bytes each and returns a pointer to the allocated memory. The memory
 * is set to zero. If nmemb or size is 0, then arena_calloc() returns a unique
 * pointer value. */
void *arena_calloc(Arena *arena, size_t nmemb, size_t size);

#ifdef ARENA_H_IMPLEMENTATION
void *arena_calloc(Arena *arena, size_t nmemb, size_t size) {
  if (nmemb == 0 || size == 0)
    return arena_alloc(arena, 0);

  size_t alloc_size = nmemb * size;
  if (alloc_size / size != nmemb) {
    return NULL;
  }

  void *ptr = arena_alloc(arena, alloc_size);
  return memset(ptr, 0, alloc_size);
}
#endif

/* The alloc_free() function frees the entire arena allocator, which
 * must have been returned by a previous call to arena_new().
 * Otherwise, or if ptr has already been freed, undefined behavior
 * occurs. If arena is NULL, no operation is performed. */
void *arena_free(Arena *arena);

#ifdef ARENA_H_IMPLEMENTATION
void *arena_free(Arena *arena) {
  if (arena != NULL)
    free(arena->data);
}
#endif

#endif
