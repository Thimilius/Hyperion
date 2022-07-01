//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/core/memory/memory.hpp"

#ifdef HYP_DEBUG
//--------------------------------------------------------------
void *operator new(size_t size) {
  Hyperion::MemoryStats::s_global_memory += size;
  Hyperion::MemoryStats::s_frame_memory += size;

  void *memory = std::malloc(size + sizeof(size_t));
  size_t *pointer = reinterpret_cast<size_t *>(memory);
  *pointer = size;

  return reinterpret_cast<void *>(pointer + 1);
}

//--------------------------------------------------------------
void operator delete(void *memory) noexcept {
  size_t *pointer = reinterpret_cast<size_t *>(memory) - 1;
  size_t size = *pointer;

  Hyperion::MemoryStats::s_global_memory -= size;
  Hyperion::MemoryStats::s_frame_memory -= size;

  std::free(pointer);
}

void *operator new[](size_t size) {
  Hyperion::MemoryStats::s_global_memory += size;
  Hyperion::MemoryStats::s_frame_memory += size;

  void *memory = std::malloc(size + sizeof(size_t));
  size_t *pointer = reinterpret_cast<size_t *>(memory);
  *pointer = size;

  return reinterpret_cast<void *>(pointer + 1);
}

void operator delete[](void *memory) noexcept {
  if (memory) {
    size_t *pointer = reinterpret_cast<size_t *>(memory) - 1;
    size_t size = *pointer;

    Hyperion::MemoryStats::s_global_memory -= size;
    Hyperion::MemoryStats::s_frame_memory -= size;

    std::free(pointer);
  }
}
#endif
