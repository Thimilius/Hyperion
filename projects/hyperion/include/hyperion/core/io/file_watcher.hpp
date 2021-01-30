#pragma once

#include "hyperion/common.hpp"

namespace Hyperion {

    enum class FileStatus {
        None,
        Created,
        Modified,
        Deleted
    };

    // NOTE: Currently we only allow to watch one directory at a time which may or may not be changed in the future.

    class FileWatcher {
    public:
        using WatcherCallbackFunction = std::function<void(FileStatus, const String &, const String &, const String &)>;
    public:
        virtual ~FileWatcher() = default;

        virtual void Update() = 0;

        static FileWatcher *Create(const String &path, WatcherCallbackFunction callback, bool recursive);
    protected:
        String m_path;
        WatcherCallbackFunction m_callback;
        bool m_recursive;
    };
}