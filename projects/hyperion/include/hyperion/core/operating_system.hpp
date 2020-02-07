#pragma once

#include "hyperion/common.hpp"

namespace Hyperion {

    enum class OperatingSystemType {
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

    enum class SystemLanguage {
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

    enum class SystemFolder {
        Desktop,
        Downloads,
        Documents,
        Pictures,
        Music,
        Videos
    };

    class OperatingSystem {
    private:
        static OperatingSystem *s_instance;
    public:
        virtual OperatingSystemType GetType() const = 0;
        virtual SystemInfo GetSystemInfo() const = 0;
        virtual u64 GetMemoryUsage() const = 0;
        virtual SystemLanguage GetSystemLanguage() const = 0;
        virtual String GetSystemFolder(SystemFolder system_folder) const = 0;

        virtual void PrintToConsole(LogColor color, const String &message) = 0;
        virtual void DisplayError(const String &title, const String &message) = 0;

        virtual String OpenFileDialog(const String &title, const String &filter) = 0;
        virtual String SaveFileDialog(const String &title, const String &filter) = 0;

        inline static OperatingSystem* GetInstance() { return s_instance; }
    private:
        virtual void Init() = 0;

        friend class Engine;
    };

}