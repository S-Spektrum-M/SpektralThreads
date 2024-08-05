#include "malloc.hpp"
#include <cstdlib>
#include <cstring>
#include <thread>
#include <unordered_map>

// TODO: Use a mutex to protect the memory map.
struct {
    std::unordered_map<void *, size_t> memory_map;
    size_t used_memory = 0;
} mallocer;

// Simple wrapper for std::async.
auto detr(auto fn) { std::thread(fn).detach(); }

void *zmalloc(size_t size) {
    void *p = malloc(size);
    detr([size, p]() {
        mallocer.used_memory += size;
        mallocer.memory_map[p] = size;
    });
    return p;
}

void *zrealloc(void *ptr, size_t size) {
    void *p = zmalloc(size);
    memcpy(p, ptr, mallocer.memory_map[ptr]);
    zfree(ptr);
    return p;
}

void zfree(void *ptr) {
    detr([ptr]() {
        mallocer.used_memory -= mallocer.memory_map[ptr];
        mallocer.memory_map.erase(ptr);
    });
    free(ptr);
}

size_t used_memory(void) { return mallocer.used_memory; }
