#pragma once

//---------------------- Library Includes ----------------------
#include <Windows.h>

//---------------------- Project Includes ----------------------
#include "hyperion/common.hpp"
#include "hyperion/core/io/file_watcher.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    class WindowsFileWatcher : public FileWatcher {
    public:
        WindowsFileWatcher(const String &path, FileWatcher::WatcherCallbackFunction callback, bool8 recursive);
        ~WindowsFileWatcher() override;
    public:
        void Update() override;
    private:
        bool8 RefreshWatch(bool8 clear);
        void HandleAction(uint32 action, const String &path, const String &filename, const String &extension);
    private:
        static void CALLBACK WatchCallback(DWORD error_code, DWORD number_of_bytes_transfered, LPOVERLAPPED overlapped);
    private:
        struct WatchStruct {
            OVERLAPPED overlapped;
            HANDLE directory_handle;
            BYTE buffer[32 * 1024];
            DWORD notify_filter;
            bool8 stop_now;
            WindowsFileWatcher *watcher; 
        };

        WatchStruct watch_struct;
    };

}