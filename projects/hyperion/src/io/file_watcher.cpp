#include "hyppch.hpp"

#include "hyperion/io/file_watcher.hpp"

#ifdef HYP_PLATFORM_WINDOWS
    #include "hyperion/platform/windows/windows_file_watcher.hpp"
#endif

namespace Hyperion {

    Ref<FileWatcher> FileWatcher::Create(const String &path, WatcherCallbackFunc callback, bool recursive) {
        switch (OperatingSystem::GetInstance()->GetType()) {
            case OperatingSystemType::Windows: return std::make_shared<WindowsFileWatcher>(path, callback, recursive);
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return nullptr;
        }
    }

}