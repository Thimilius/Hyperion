#include "hyppch.hpp"

#include "windows_file_watcher.hpp"

namespace Hyperion::IO {

    CWindowsFileWatcher::CWindowsFileWatcher(const TString &path, WatcherCallbackFunc callback, bool recursive) {
        m_path = path;
        m_callback = callback;
        m_recursive = recursive;

        watch_struct.directory_handle = CreateFileA(
            path.c_str(),
            FILE_LIST_DIRECTORY,
            FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
            NULL,
            OPEN_EXISTING,
            FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED,
            NULL);

        if (watch_struct.directory_handle != INVALID_HANDLE_VALUE) {
            watch_struct.overlapped.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
            watch_struct.notify_filter = FILE_NOTIFY_CHANGE_CREATION | FILE_NOTIFY_CHANGE_LAST_WRITE | FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME;
            watch_struct.watcher = this;
            watch_struct.stop_now = false;

            if (!RefreshWatch(false)) {
                CloseHandle(watch_struct.overlapped.hEvent);
                CloseHandle(watch_struct.directory_handle);
            }
        }
    }

    CWindowsFileWatcher::~CWindowsFileWatcher() {
        CancelIo(watch_struct.directory_handle);

        RefreshWatch(true);

        watch_struct.stop_now = true;

        if (!HasOverlappedIoCompleted(&watch_struct.overlapped)) {
            SleepEx(1, TRUE);
        }

        CloseHandle(watch_struct.overlapped.hEvent);
        CloseHandle(watch_struct.directory_handle);
    }

    void CWindowsFileWatcher::Update() {
        MsgWaitForMultipleObjectsEx(0, NULL, 0, QS_ALLINPUT, MWMO_ALERTABLE);
    }

    bool CWindowsFileWatcher::RefreshWatch(bool clear) {
        BOOL result = ReadDirectoryChangesW(
            watch_struct.directory_handle,
            watch_struct.buffer,
            sizeof(watch_struct.buffer),
            m_recursive,
            watch_struct.notify_filter,
            NULL,
            &watch_struct.overlapped,
            clear ? 0 : WatchCallback);
        return result != 0;
    }

    void CWindowsFileWatcher::HandleAction(const TString &path, u32 action) {
        EFileStatus status;
        switch (action) {
            case FILE_ACTION_RENAMED_NEW_NAME:
            case FILE_ACTION_ADDED:
                status = EFileStatus::Created;
                break;
            case FILE_ACTION_RENAMED_OLD_NAME:
            case FILE_ACTION_REMOVED:
                status = EFileStatus::Deleted;
                break;
            case FILE_ACTION_MODIFIED:
                status = EFileStatus::Modified;
                break;
        }

        m_callback(status, path);
    }

    void CALLBACK CWindowsFileWatcher::WatchCallback(DWORD error_code, DWORD number_of_bytes_transfered, LPOVERLAPPED overlapped) {
        char file[MAX_PATH];
        FILE_NOTIFY_INFORMATION *notify;
        SWatchStruct *watch_struct = (SWatchStruct *)overlapped;
        size_t offset = 0;

        if (number_of_bytes_transfered == 0) {
            return;
        }

        if (error_code == ERROR_SUCCESS) {
            do {
                notify = (FILE_NOTIFY_INFORMATION*)&watch_struct->buffer[offset];
                offset += notify->NextEntryOffset;

                int count = WideCharToMultiByte(CP_ACP, 0, notify->FileName, notify->FileNameLength / sizeof(WCHAR), file, MAX_PATH - 1, NULL, NULL);
                file[count] = '\0';

                TString path = CStringUtils::Format("{}{}", watch_struct->watcher->m_path, file);

                Sleep(1);
                watch_struct->watcher->HandleAction(path, notify->Action);
            } while (notify->NextEntryOffset != 0);
        }

        if (!watch_struct->stop_now) {
            watch_struct->watcher->RefreshWatch(false);
        }
    }
}