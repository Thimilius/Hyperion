#include "hyppch.hpp"

#include "hyperion/system/file_watcher.hpp"

namespace Hyperion {

    CFileWatcher::CFileWatcher(const TString &path, FileChangedCallbackFunc callback, bool recursive) {
        m_path = path;
        m_callback = callback;
        m_recursive = recursive;

        if (m_recursive) {
            for (const auto &entry : std::filesystem::recursive_directory_iterator(path)) {
                m_paths[entry.path().string()] = std::filesystem::last_write_time(entry);
            }
        } else {
            for (const auto &entry : std::filesystem::directory_iterator(path)) {
                m_paths[entry.path().string()] = std::filesystem::last_write_time(entry);
            }
        }
    }

    void CFileWatcher::Update() {
        if (!m_enabled) {
            return;
        }

        auto it = m_paths.begin();
        while (it != m_paths.end()) {
            if (std::filesystem::exists(it->first)) {
                it++;
            } else {
                // Deletion
                m_callback(EFileStatus::Deleted, it->first);
                it = m_paths.erase(it);
            }                    
        }
 
        // Check if a file was created or modified
        if (m_recursive) {
            for (const auto &entry : std::filesystem::recursive_directory_iterator(m_path)) {
                ProcessEntry(entry);
            }
        } else {
            for (const auto &entry : std::filesystem::directory_iterator(m_path)) {
                ProcessEntry(entry);
            }
        }
    }

    void CFileWatcher::ProcessEntry(const std::filesystem::directory_entry &entry) {
        auto last_write_time = std::filesystem::last_write_time(entry);

        if (m_paths.find(entry.path().string()) == m_paths.end()) {
            // Creation
            m_paths[entry.path().string()] = last_write_time;
            m_callback(EFileStatus::Created, entry.path().string());
        } else {
            // Modification
            if (m_paths[entry.path().string()] != last_write_time) {
                m_paths[entry.path().string()] = last_write_time;
                m_callback(EFileStatus::Modified, entry.path().string());
            }
        }
    }

}