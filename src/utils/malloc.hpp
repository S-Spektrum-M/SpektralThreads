#pragma once
#include <cstddef>

[[nodiscard("Discarding may lead to memory leaks")]] void *zmalloc(size_t size);
[[nodiscard("Discarding may lead to memory leaks")]] void *
zrealloc(void *ptr, size_t size);
void zfree(void *ptr);
size_t used_memory(void);
