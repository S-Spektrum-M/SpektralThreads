#include "malloc.hpp"
#include <cstdlib>
#include <cstring>
#include <thread>
#include <unordered_map>

// TODO: Use a mutex to protect the memory map.
struct {
    std::unordered_map<void *, size_t> memory_map;
    size_t used_memory = 0;
} tracker;

// Simple wrapper for std::async.
auto detr(auto fn) { std::thread(fn).detach(); }

void *zmalloc(size_t size) {
    void *p = malloc(size);
    detr([size, p]() {
        tracker.used_memory += size;
        tracker.memory_map[p] = size;
    });
    return p;
}

void *zrealloc(void *ptr, size_t size) {
    void *p = zmalloc(size);
    memcpy(p, ptr, tracker.memory_map[ptr]);
    zfree(ptr);
    return p;
}

void zfree(void *ptr) {
    detr([ptr]() {
        tracker.used_memory -= tracker.memory_map[ptr];
        tracker.memory_map.erase(ptr);
    });
    free(ptr);
}

size_t used_memory(void) { return tracker.used_memory; }
