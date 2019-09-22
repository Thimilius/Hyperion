#include "hyppch.hpp"

#include "hyperion/io/file_utilities.hpp"

namespace Hyperion::IO {

    TString CFileUtilities::ReadTextFile(const TString &path) {
        HANDLE file_handle = CreateFileA(path.c_str(), GENERIC_READ, FILE_SHARE_WRITE | FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (file_handle == INVALID_HANDLE_VALUE) {
            HYP_CORE_ERROR("Failed to open file: '{}'!", path);
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

        TString result = TString(file_buffer);
        delete[] file_buffer;
        return result;
    }

}