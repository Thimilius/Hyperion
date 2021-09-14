#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/types.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    enum class OperatingSystemType {
        Windows
    };

    struct OperatingSystemInfo {
        struct ProcessorInfo {
            uint32 processor_count;
            uint32 processor_mhz_frequency;
        } processor_info;

        struct MemoryInfo {
            uint64 available_physical_memory;
            uint64 total_physical_memory;

            uint64 available_virtual_memory;
            uint64 total_virtual_memory;
        } memory_info;
    };

    enum class OperatingSystemLanguage {
        Unknown,

        Albanian,
        Arabic,
        Armenian,
        Belarusian,
        Bulgarian,
        Catalan,
        Chinese,
        Croatian,
        Czech,
        Danish,
        Dutch,
        English,
        Estonian,
        Finnish,
        French,
        German,
        Greek,
        Hebrew,
        Hindi,
        Hungarian,
        Icelandic,
        Indonesian,
        Irish,
        Italian,
        Japanese,
        Korean,
        Lativian,
        Lithuanian,
        Macedonian,
        Malay,
        Maltese,
        Norwegian,
        Polish,
        Portuguese,
        Romanian,
        Russian,
        Slovak,
        Slovenian,
        Spanish,
        Swedish,
        Thai,
        Turkish,
        Ukrainian,
        Vietnamese
    };

    enum class OperatingSystemSpecialFolder {
        Desktop,
        Downloads,
        Documents,
        Pictures,
        Music,
        Videos
    };

}