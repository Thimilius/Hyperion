#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/system/log.hpp"
#include "hyperion/core/assert.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    enum class Error {
        Unknown,

        FileDoesNotExist,
        ImageLoadFailed,
        NoLoaderAvailable,
    };

    inline String ToString(Error error) {
        switch (error) {
            case Error::Unknown: return "Unknown";
            case Error::FileDoesNotExist: return "FileDoesNotExist";
            case Error::ImageLoadFailed: return "ImageLoadFailed";
            case Error::NoLoaderAvailable: return "NoLoaderAvailable";
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return {};
        }
    }

}