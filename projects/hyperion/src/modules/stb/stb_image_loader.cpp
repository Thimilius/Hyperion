#include "hyppch.hpp"

#include "hyperion/modules/stb/stb_image_format_loader.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace Hyperion {

    bool StbImageFormatLoader::SupportsExtension(const String &extension) const {
        return std::find(s_supported_extensions.begin(), s_supported_extensions.end(), extension) != s_supported_extensions.end();
    }

    Image *StbImageFormatLoader::Load(const String &path, bool flip_vertically) {
        stbi_set_flip_vertically_on_load(flip_vertically);

        s32 width;
        s32 height;
        s32 channels;
        u8 *pixels = stbi_load(path.c_str(), &width, &height, &channels, 0);

        if (!pixels) {
            HYP_LOG_ERROR("Engine", "Failed to load image from path: {}", std::filesystem::absolute(path).u8string());
            width = 0;
            height = 0;
            channels = 0;
        }

        return Image::Create(width, height, channels, pixels);
    }

}