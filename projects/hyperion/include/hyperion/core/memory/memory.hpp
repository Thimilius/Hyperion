#pragma once

//--------------- C++ Standard Library Includes ----------------
#include <atomic>

//---------------------- Project Includes ----------------------
#include "hyperion/core/types.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion {
    class Engine;
    class Main;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    class MemoryStats final {
    public:
        inline static uint64 GetGlobalMemory() { return s_global_memory; }
        inline static uint64 GetFrameMemory() { return s_frame_memory; }
    private:
        MemoryStats() = delete;
        ~MemoryStats() = delete;
    private:
        inline static void ResetFrameMemory() { s_frame_memory = 0; }
    private:
        inline static std::atomic_uint64_t s_global_memory = 0;
        inline static std::atomic_uint64_t s_frame_memory = 0;
    private:
        friend class Hyperion::Engine;
        friend class Hyperion::Main;

        friend void *::operator new(size_t size);
        friend void ::operator delete(void *memory, size_t size);
    };

}