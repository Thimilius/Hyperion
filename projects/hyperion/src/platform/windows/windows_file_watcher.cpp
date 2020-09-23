#include "hyppch.hpp"

#include "hyperion/platform/windows/windows_file_watcher.hpp"

namespace Hyperion {

    Ref<FileWatcher> FileWatcher::Create(const String &path, WatcherCallbackFunction callback, bool recursive) {
        return std::make_shared<WindowsFileWatcher>(path, callback, recursive);
    }

    WindowsFileWatcher::WindowsFileWatcher(const String &path, WatcherCallbackFunction callback, bool recursive) {
        m_path = path;
        m_callback = callback;
        m_recursive = recursive;

        watch_struct.directory_handle = CreateFileA(
            path.c_str(),
            FILE_LIST_DIRECTORY,
            FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
            nullptr,
            OPEN_EXISTING,
            FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED,
            nullptr);

        if (watch_struct.directory_handle != INVALID_HANDLE_VALUE) {
            watch_struct.overlapped.hEvent = CreateEvent(nullptr, TRUE, FALSE, nullptr);
            watch_struct.notify_filter = FILE_NOTIFY_CHANGE_CREATION | FILE_NOTIFY_CHANGE_LAST_WRITE | FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME;
            watch_struct.watcher = this;
            watch_struct.stop_now = false;

            if (!RefreshWatch(false)) {
                CloseHandle(watch_struct.overlapped.hEvent);
                CloseHandle(watch_struct.directory_handle);
            }
        }
    }

    WindowsFileWatcher::~WindowsFileWatcher() {
        CancelIo(watch_struct.directory_handle);

        RefreshWatch(true);

        watch_struct.stop_now = true;

        if (!HasOverlappedIoCompleted(&watch_struct.overlapped)) {
            SleepEx(1, TRUE);
        }

        CloseHandle(watch_struct.overlapped.hEvent);
        CloseHandle(watch_struct.directory_handle);
    }

    void WindowsFileWatcher::Update() {
        MsgWaitForMultipleObjectsEx(0, nullptr, 0, QS_ALLINPUT, MWMO_ALERTABLE);
    }

    bool WindowsFileWatcher::RefreshWatch(bool clear) {
        BOOL result = ReadDirectoryChangesW(
            watch_struct.directory_handle,
            watch_struct.buffer,
            sizeof(watch_struct.buffer),
            m_recursive,
            watch_struct.notify_filter,
            nullptr,
            &watch_struct.overlapped,
            clear ? 0 : WatchCallback);
        return result != 0;
    }

    void WindowsFileWatcher::HandleAction(u32 action, const String &path, const String &filename, const String &extension) {
        FileStatus status;
        switch (action) {
            case FILE_ACTION_RENAMED_NEW_NAME:
            case FILE_ACTION_ADDED:
                status = FileStatus::Created;
                break;
            case FILE_ACTION_RENAMED_OLD_NAME:
            case FILE_ACTION_REMOVED:
                status = FileStatus::Deleted;
                break;
            case FILE_ACTION_MODIFIED:
                status = FileStatus::Modified;
                break;
        }

        m_callback(status, path, filename, extension);
    }

    void CALLBACK WindowsFileWatcher::WatchCallback(DWORD error_code, DWORD number_of_bytes_transfered, LPOVERLAPPED overlapped) {
        FILE_NOTIFY_INFORMATION *notify;
        WatchStruct *watch_struct = (WatchStruct *)overlapped;
        size_t notify_offset = 0;

        if (number_of_bytes_transfered == 0) {
            return;
        }

        if (error_code == ERROR_SUCCESS) {
            do {
                notify = (FILE_NOTIFY_INFORMATION*)&watch_struct->buffer[notify_offset];
                notify_offset += notify->NextEntryOffset;
                
                // File name length is in bytes and therefore needs to be converted
                auto filename_length = notify->FileNameLength / 2;
                String filename = StringUtils::Utf16ToUtf8(WideString(notify->FileName).substr(0, filename_length));

                // Format path to always include last directory seperator
                bool has_seperator = false;
                if (watch_struct->watcher->m_path.back() == '\\' || watch_struct->watcher->m_path.back() == '/') {
                    has_seperator = true;
                }
                String path = has_seperator ?
                    StringUtils::Format("{}{}", watch_struct->watcher->m_path, filename) :
                    StringUtils::Format("{}/{}", watch_struct->watcher->m_path, filename);
                
                String extension = filename.substr(filename.find_last_of("."));

                // HACK: This is a pretty nasty hack of trying to "wait" for long enough,
                // so that file changes are actually written to disk and
                // do not cause any weird errors when trying to load
                Sleep(2);
                 
                watch_struct->watcher->HandleAction(notify->Action, path, filename, extension);
            } while (notify->NextEntryOffset != 0);
        }

        if (!watch_struct->stop_now) {
            watch_struct->watcher->RefreshWatch(false);
        }
    }
}