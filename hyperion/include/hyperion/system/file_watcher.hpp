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

    class CFileWatcher {
        using FileChangedCallbackFunc = std::function<void(EFileStatus, const TString &)>;
    private:
        bool m_enabled;

        TString m_path;
        FileChangedCallbackFunc m_callback;
        bool m_recursive;

        TMap<TString, std::filesystem::file_time_type> m_paths;
    public:
        CFileWatcher(const TString &path, FileChangedCallbackFunc callback, bool recursive);

        void Enable() { m_enabled = true; }
        void Disable() { m_enabled = false; }
        bool IsEnabled() const { return m_enabled; }

        void Update();
    private:
        void ProcessEntry(const std::filesystem::directory_entry &entry);
    };

}