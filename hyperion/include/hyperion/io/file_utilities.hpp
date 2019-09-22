#pragma once

#include "hyperion/common.hpp"

namespace Hyperion::IO {

    class CFileUtilities {
    public:
        static TString ReadTextFile(const TString &path);
    private:
        CFileUtilities() = delete;
        ~CFileUtilities() = delete;
    };

}