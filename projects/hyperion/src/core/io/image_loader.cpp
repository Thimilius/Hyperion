#include "hyppch.hpp"

#include "hyperion/core/io/image_loader.hpp"

#include "hyperion/modules/stb/stb_image_format_loader.hpp"

namespace Hyperion {

    Scope<ImageFormatLoader> ImageLoader::m_loader = std::make_unique<StbImageFormatLoader>();

    const Vector<String> &ImageLoader::GetSupportedExtensions() {
        return m_loader->GetSupportedExtensions();
    }

    bool ImageLoader::SupportsExtension(const String &extension) {
        return m_loader->SupportsExtension(extension);
    }

    Ref<Image> ImageLoader::Load(const String &path, bool flip_vertically) {
        return m_loader->Load(path, flip_vertically);
    }

}