//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/common.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    class TextureUtilities final {
    public:
        static uint32 CalculateMipmapCount(uint32 width, uint32 height);
    private:
        TextureUtilities() = delete;
        ~TextureUtilities() = delete;
    };

}