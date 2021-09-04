//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/modules/stb/stb_image_loader.hpp"

//---------------------- Library Includes ----------------------
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

//---------------------- Project Includes ----------------------
#include "hyperion/core/io/file_system.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    //--------------------------------------------------------------
    bool8 StbImageLoader::SupportsExtension(const String &extension) const {
        return std::find(s_supported_extensions.begin(), s_supported_extensions.end(), extension) != s_supported_extensions.end();
    }

    //--------------------------------------------------------------
    Result<Image *, Error> StbImageLoader::Load(const String &path, bool8 flip_vertically) {
        if (!FileSystem::Exists(path)) {
            return { Error::FileDoesNotExist };
        }

        stbi_set_flip_vertically_on_load(flip_vertically);

        int32 width = 0;
        int32 height = 0;
        int32 channels = 0;
        byte *buffer = stbi_load(path.c_str(), &width, &height, &channels, 0);

        if (!buffer) {
            return { Error::ImageLoadFailed };
        }

        List<byte> pixels(buffer, buffer + (width * height * channels));
        stbi_image_free(buffer);

        return { Image::Create(width, height, channels, std::move(pixels)) };
    }

}