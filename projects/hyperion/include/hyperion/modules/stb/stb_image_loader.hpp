#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/io/image_loader.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    class StbImageLoader : public IImageLoader {
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