#pragma once

#include "hyperion/common.hpp"

namespace Hyperion {

    enum class FileStatus {
        None,
        Created,
        Modified,
        Deleted
    };

    // Currently we only allow to watch one directory at a time which may or may not be changed in the future

    class FileWatcher {
        using WatcherCallbackFunc = std::function<void(FileStatus, const String &, const String &, const String &)>;
    public:
        virtual ~FileWatcher() = default;

        virtual void Update() = 0;

        static Ref<FileWatcher> Create(const String &path, WatcherCallbackFunc callback, bool recursive);
    protected:
        String m_path;
        WatcherCallbackFunc m_callback;
        bool m_recursive;
    };
}