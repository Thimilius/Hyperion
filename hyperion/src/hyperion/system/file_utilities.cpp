#include "hyppch.hpp"

#include "hyperion/system/file_utilities.hpp"

#include <fstream>

namespace Hyperion {

    TString CFileUtilities::ReadFile(const TString &path) {
        std::ifstream file(path);
        if (!file) {
            HYP_CORE_ERROR("Failed to open file: {}!", path);
            return TString();
        }

        file.seekg(0, file.end);
        u32 length = (u32)file.tellg();
        file.seekg(0, file.beg);

        // Reserve memory
        TString result(length, ' ');

        file.read(&result[0], length);
        file.close();

        if (!file.eof()) {
            HYP_CORE_ERROR("Failed to read entire file: {}!", path);
            return TString();
        }

        return result;
    }

}