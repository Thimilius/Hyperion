#include "hyppch.hpp"

#include "hyperion/core/io/image_loader.hpp"

#include "hyperion/modules/stb/stb_image_format_loader.hpp"

namespace Hyperion {

    Vector<String> ImageLoader::GetSupportedExtensions() {
        Vector<String> extensions;

        return extensions;
    }

    bool ImageLoader::SupportsExtension(const String &extension) {
        for (IImageFormatLoader *image_format_loader : s_loaders) {
            if (image_format_loader->SupportsExtension(extension)) {
                return true;
            }
        }
        return false;
    }

    Ref<Image> ImageLoader::Load(const String &path, bool flip_vertically) {
        for (IImageFormatLoader *image_format_loader : s_loaders) {
            if (image_format_loader->SupportsExtension(StringUtils::GetExtension(path))) {
                return image_format_loader->Load(path, flip_vertically);
            }
        }
        return nullptr;
    }

    void ImageLoader::AddFormatLoader(IImageFormatLoader *image_format_loader) {
        s_loaders.push_back(image_format_loader);
    }

    void ImageLoader::Init() {
        AddFormatLoader(new StbImageFormatLoader());
    }

    void ImageLoader::Shutdown() {
        for (IImageFormatLoader *image_format_loader : s_loaders) {
            delete image_format_loader;
        }
    }

}