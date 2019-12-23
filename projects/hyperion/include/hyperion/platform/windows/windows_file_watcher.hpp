#pragma once

#include "hyperion/common.hpp"
#include "hyperion/core/io/file_watcher.hpp"

#include <Windows.h>

namespace Hyperion {

    class WindowsFileWatcher : public FileWatcher {
        using WatcherCallbackFunc = std::function<void(FileStatus, const String &, const String &, const String &)>;
    private:
        struct WatchStruct {
            OVERLAPPED overlapped;
            HANDLE directory_handle;
            BYTE buffer[32 * 1024];
            DWORD notify_filter;
            bool stop_now;
            WindowsFileWatcher *watcher; 
        };

        WatchStruct watch_struct;
    public:
        WindowsFileWatcher(const String &path, WatcherCallbackFunc callback, bool recursive);
        ~WindowsFileWatcher() override;

        void Update() override;
    private:
        bool RefreshWatch(bool clear);
        void HandleAction(u32 action, const String &path, const String &filename, const String &extension);

        static void CALLBACK WatchCallback(DWORD error_code, DWORD number_of_bytes_transfered, LPOVERLAPPED overlapped);
    };

}