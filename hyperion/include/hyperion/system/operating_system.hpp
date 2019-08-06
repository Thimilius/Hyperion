#pragma once

#include "hyperion/common.hpp"

namespace Hyperion {

    enum class OperatingSystemType {
        Unknown,
        Windows
    };

    struct SystemInfo {
        struct ProcessorInfo {
            u32 processor_count;
            u32 processor_mhz_frequency;
        } processor_info;

        struct MemoryInfo {
            u64 available_physical_memory;
            u64 total_physical_memory;

            u64 available_virtual_memory;
            u64 total_virtual_memory;
        } memory_info;
    };

    class OperatingSystem {
    private:
        static OperatingSystem *s_instance;
    public:
        virtual OperatingSystemType GetType() const = 0;
        virtual SystemInfo GetSystemInfo() const = 0;

        static OperatingSystem* Get() { return s_instance; }
    };

}