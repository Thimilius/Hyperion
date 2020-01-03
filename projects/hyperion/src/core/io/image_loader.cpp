#include "hyppch.hpp"

#include "hyperion/core/io/image_loader.hpp"

#include "hyperion/driver/stb/stb_image_loader.hpp"

namespace Hyperion {

    Scope<ImageLoader> ImageLoader::m_loader = std::make_unique<StbImageLoader>();

    Ref<Image> ImageLoader::Load(const String &path, bool flip_vertically) {
        return m_loader->LoadFromFile(path, flip_vertically);
    }

}