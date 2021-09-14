#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/assert.hpp"
#include "hyperion/core/log.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    enum class Error {
        None,

        FileDoesNotExist,
        ImageLoadFailed,
        NoLoaderAvailable,

        InvalidMesh,
    };

    inline String ToString(Error error) {
        switch (error) {
            case Error::None: return "None";
            case Error::FileDoesNotExist: return "FileDoesNotExist";
            case Error::ImageLoadFailed: return "ImageLoadFailed";
            case Error::NoLoaderAvailable: return "NoLoaderAvailable";
            case Error::InvalidMesh: return "InvalidMesh";
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return {};
        }
    }

}