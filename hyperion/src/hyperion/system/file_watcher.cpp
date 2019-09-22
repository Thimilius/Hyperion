#include "hyppch.hpp"

#include "hyperion/system/file_watcher.hpp"

#ifdef HYP_PLATFORM_WINDOWS
    #include "hyperion/platform/windows/windows_file_watcher.hpp"
#endif

namespace Hyperion {

    TRef<CFileWatcher> CFileWatcher::Create(const TString &path, WatcherCallbackFunc callback, bool recursive) {
        switch (COperatingSystem::GetInstance()->GetType()) {
            case EOperatingSystemType::Windows: return std::make_shared<CWindowsFileWatcher>(path, callback, recursive);
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return nullptr;
        }
    }

}