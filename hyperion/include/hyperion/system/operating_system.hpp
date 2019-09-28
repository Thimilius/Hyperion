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

        // NOTE: This should probably be in a different place?
        virtual TString ConvertUTF16ToUTF8(const TWString &string) const = 0;
        virtual TWString ConvertUTF8ToUTF16(const TString &string) const = 0;

        virtual void PrintToConsole(ELogColor color, const TString &message) const = 0;

        inline static COperatingSystem* GetInstance() { return s_instance; }
    private:
        virtual void Init() = 0;

        friend class CEngine;
    };

}