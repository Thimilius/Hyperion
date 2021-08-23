//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/modules/stb/stb_image_loader.hpp"

//---------------------- Library Includes ----------------------
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    //--------------------------------------------------------------
    bool StbImageLoader::SupportsExtension(const String &extension) const {
        return std::find(s_supported_extensions.begin(), s_supported_extensions.end(), extension) != s_supported_extensions.end();
    }

    //--------------------------------------------------------------
    Image *StbImageLoader::Load(const String &path, bool flip_vertically) {
        stbi_set_flip_vertically_on_load(flip_vertically);

        int32 width = 0;
        int32 height = 0;
        int32 channels = 0;
        byte *buffer = stbi_load(path.c_str(), &width, &height, &channels, 0);

        if (!buffer) {
            HYP_LOG_ERROR("Engine", "Failed to load image from path: {}", std::filesystem::absolute(path).u8string());
            return nullptr;
        }

        Vector<byte> pixels(buffer, buffer + (width * height * channels));
        stbi_image_free(buffer);

        return Image::Create(width, height, channels, std::move(pixels));
    }

}