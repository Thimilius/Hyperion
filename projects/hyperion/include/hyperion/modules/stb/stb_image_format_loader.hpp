#pragma once

#include "hyperion/core/io/image_loader.hpp"

namespace Hyperion {

    class StbImageFormatLoader : public IImageFormatLoader {
    public:
        inline const Vector<String> &GetSupportedExtensions() const override { return s_supported_extensions; }
        bool SupportsExtension(const String &extension) const override;

        Image *Load(const String &path, bool flip_vertically) override;
    private:
        inline static Vector<String> s_supported_extensions = {
            ".png",
            ".jpg",
            ".jpeg",
            ".gif",
            ".bmp",
            ".tga",
            ".psd"
        };
    };

}