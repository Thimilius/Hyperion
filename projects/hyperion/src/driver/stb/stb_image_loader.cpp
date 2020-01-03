#include "hyppch.hpp"

#include "hyperion/driver/stb/stb_image_loader.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

namespace Hyperion {

    Ref<Image> StbImageLoader::LoadFromFile(const String &path, bool flip_vertically) {
        stbi_set_flip_vertically_on_load(flip_vertically);

        s32 width;
        s32 height;
        s32 channels;
        u8 *pixels = stbi_load(path.c_str(), &width, &height, &channels, 0);

        if (!pixels) {
            HYP_LOG_ERROR("Engine", "Failed to load image from path: {}", path);
            width = 0;
            height = 0;
            channels = 0;
        }

        return Image::Create(width, height, channels, pixels, true);
    }

}