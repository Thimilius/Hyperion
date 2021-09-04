#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/assets/loader/image_loader.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    class StbImageLoader : public IImageLoader {
    public:
        inline const List<String> &GetSupportedExtensions() const override { return s_supported_extensions; }
        bool8 SupportsExtension(const String &extension) const override;

        Result<Image *, Error> Load(const String &path, bool8 flip_vertically) override;
    private:
        inline static List<String> s_supported_extensions = {
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