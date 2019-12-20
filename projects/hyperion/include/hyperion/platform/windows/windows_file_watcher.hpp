#pragma once

#include "hyperion/common.hpp"
#include "hyperion/io/file_watcher.hpp"

#include <Windows.h>

namespace Hyperion::IO {

    class CWindowsFileWatcher : public CFileWatcher {
        using WatcherCallbackFunc = std::function<void(EFileStatus, const TString &, const TString &, const TString &)>;
    private:
        struct SWatchStruct {
            OVERLAPPED overlapped;
            HANDLE directory_handle;
            BYTE buffer[32 * 1024];
            DWORD notify_filter;
            bool stop_now;
            CWindowsFileWatcher *watcher; 
        };

        SWatchStruct watch_struct;
    public:
        CWindowsFileWatcher(const TString &path, WatcherCallbackFunc callback, bool recursive);
        virtual ~CWindowsFileWatcher();

        void Update() override;
    private:
        bool RefreshWatch(bool clear);
        void HandleAction(u32 action, const TString &path, const TString &filename, const TString &extension);

        static void CALLBACK WatchCallback(DWORD error_code, DWORD number_of_bytes_transfered, LPOVERLAPPED overlapped);
    };

}