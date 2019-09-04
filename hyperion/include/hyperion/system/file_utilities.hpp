#pragma once

#include "hyperion/common.hpp"

namespace Hyperion {

    class CFileUtilities {
    public:
        static TString ReadFile(const TString &path);
    private:
        CFileUtilities() = delete;
        ~CFileUtilities() = delete;
    };

}