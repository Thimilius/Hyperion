#pragma once

#include "hyperion/common.hpp"

#include <filesystem>

namespace Hyperion {

    enum class EFileStatus {
        None,
        Created,
        Modified,
        Deleted
    };

    // Currently we only allow to watch one directory at a time which may or may not be changed in the future

    class CFileWatcher {
        using WatcherCallbackFunc = std::function<void(EFileStatus, const TString &)>;
    protected:
        TString m_path;
        WatcherCallbackFunc m_callback;
        bool m_recursive;
    public:
        virtual ~CFileWatcher() = default;

        virtual void Update() = 0;

        static TRef<CFileWatcher> Create(const TString &path, WatcherCallbackFunc callback, bool recursive);
    };
}