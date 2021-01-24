#pragma once

#include <Windows.h>

#include "hyperion/common.hpp"
#include "hyperion/core/io/file_watcher.hpp"

namespace Hyperion {

    class WindowsFileWatcher : public FileWatcher {
    public:
        WindowsFileWatcher(const String &path, FileWatcher::WatcherCallbackFunction callback, bool recursive);
        ~WindowsFileWatcher() override;

        void Update() override;
    private:
        bool RefreshWatch(bool clear);
        void HandleAction(uint32 action, const String &path, const String &filename, const String &extension);

        static void CALLBACK WatchCallback(DWORD error_code, DWORD number_of_bytes_transfered, LPOVERLAPPED overlapped);
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
    };

}