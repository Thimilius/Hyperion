#include "hyppch.hpp"

void *operator new(size_t size) {
    return std::malloc(size);
}

void *operator new[](size_t size) {
    return std::malloc(size);
}

void operator delete(void *memory, size_t size) {
    std::free(memory);
}

void operator delete[](void *memory, size_t size) {
    std::free(memory);
}
