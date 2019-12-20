#include "hyppch.hpp"

#include "hyperion/core/io/file_utilities.hpp"

namespace Hyperion {

    String FileUtilities::ReadTextFile(const String &path) {
        HANDLE file_handle = CreateFileA(path.c_str(), GENERIC_READ, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (file_handle == INVALID_HANDLE_VALUE) {
            HYP_LOG_ERROR("Engine", "Failed to open file: '{}'!", path);
            return NULL;
        }

        LARGE_INTEGER file_size_large;
        GetFileSizeEx(file_handle, &file_size_large);

        u64 file_size = file_size_large.QuadPart;

        char *file_buffer = new char[file_size + 1];
        DWORD bytes_read = 0;
        ReadFile(file_handle, file_buffer, (DWORD)file_size, &bytes_read, NULL);
        file_buffer[file_size] = 0;

        CloseHandle(file_handle);

        String result = String(file_buffer);
        delete[] file_buffer;
        return result;
    }

}