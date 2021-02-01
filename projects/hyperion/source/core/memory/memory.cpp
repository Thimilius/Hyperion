#include "hyppch.hpp"

#include "hyperion/core/memory/memory.hpp"

// RANT: We would really like to replace the global new[]/delete[] operators as well,
// but it seems the delete overloads excepting a second size_t argument are broken in multiple compilers
// because they break the ABI or something. Thanks C++! Why bother putting it in the standard then?!?
// Actually I just realized when not replacing new[]/delete[] that Visual C++ calls
// the new operator defined here for new[] but not delete for delete[]! WHAT IS THAT!?!?

void *operator new(size_t size) {
#ifdef HYP_DEBUG
    Hyperion::MemoryStats::s_global_memory += size;
    Hyperion::MemoryStats::s_frame_memory += size;
#endif
    return std::malloc(size);
}

void operator delete(void *memory, size_t size) {
#ifdef HYP_DEBUG
    Hyperion::MemoryStats::s_global_memory -= size;
    Hyperion::MemoryStats::s_frame_memory -= size;
#endif
    std::free(memory);
}
