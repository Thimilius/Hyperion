#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/io/file_watcher_types.hpp"
#include "hyperion/platform/windows/windows_types.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    class WindowsFileWatcher : public INonCopyable {
    public:
        WindowsFileWatcher() = default;
        WindowsFileWatcher(const String &path, FileWatcherCallbackFunction callback, bool8 recursive);
        ~WindowsFileWatcher();
    public:
        void Update();
    private:
        bool8 RefreshWatch(bool8 clear);
        void HandleAction(uint32 action, const String &path, const String &filename, const String &extension);
    private:
        static void WIN_API_CALLCONV WatchCallback(DWORD error_code, DWORD number_of_bytes_transfered, LPOverlapped overlapped);
    private:
        String m_path;
        FileWatcherCallbackFunction m_callback;
        bool8 m_recursive;

        struct WatchStruct {
            Overlapped overlapped;
            HANDLE directory_handle;
            BYTE buffer[32 * 1024];
            DWORD notify_filter;
            bool8 stop_now;
            WindowsFileWatcher *watcher; 
        };

        WatchStruct watch_struct;
    };

}