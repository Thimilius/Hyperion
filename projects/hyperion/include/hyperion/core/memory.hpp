#pragma once

#include <atomic>

#include "hyperion/core/types.hpp"

namespace Hyperion {
    class Engine;
}

namespace Hyperion {

    class MemoryStats final {
    public:
        inline static uint64 GetGlobalMemory() { return s_global_memory; }
        inline static uint64 GetFrameMemory() { return s_frame_memory; }
    private:
        MemoryStats() = delete;
        ~MemoryStats() = delete;

        inline static void ResetFrameMemory() { s_frame_memory = 0; }
    private:
        inline static std::atomic_uint64_t s_global_memory;
        inline static std::atomic_uint64_t s_frame_memory;
    private:
        friend class Hyperion::Engine;

        friend void *::operator new(size_t size);
        friend void *::operator new[](size_t size);
        friend void ::operator delete(void *memory, size_t size);
        friend void ::operator delete[](void *memory, size_t size);
    };

}