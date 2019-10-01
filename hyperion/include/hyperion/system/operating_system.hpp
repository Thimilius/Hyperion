#pragma once

#include "hyperion/common.hpp"

namespace Hyperion {

    enum class EOperatingSystemType {
        Unknown,
        Windows
    };

    struct SSystemInfo {
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

    class COperatingSystem {
    private:
        static COperatingSystem *s_instance;
    public:
        virtual EOperatingSystemType GetType() const = 0;
        virtual SSystemInfo GetSystemInfo() const = 0;

        virtual void PrintToConsole(ELogColor color, const TString &message) const = 0;

        virtual void DisplayError(const TString &title, const TString &message) const = 0;

        inline static COperatingSystem* GetInstance() { return s_instance; }
    private:
        virtual void Init() = 0;

        friend class CEngine;
    };

}