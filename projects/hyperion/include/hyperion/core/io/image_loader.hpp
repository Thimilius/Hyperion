#pragma once

#include "hyperion/core/image.hpp"

namespace Hyperion {
    class AssetManager;
}

namespace Hyperion {

    class IImageLoader {
    public:
        virtual ~IImageLoader() = default;
    public:
        virtual const Vector<String> &GetSupportedExtensions() const = 0;
        virtual bool SupportsExtension(const String &extension) const = 0;

        virtual Image *Load(const String &path, bool flip_vertically) = 0;
    };

    class ImageLoader final {
    public:
        static Vector<String> GetSupportedExtensions();
        static bool SupportsExtension(const String &extension);

        static Image *Load(const String &path, bool flip_vertically = false);
        static void AddFormatLoader(IImageLoader *image_loader);
    private:
        ImageLoader() = delete;
        ~ImageLoader() = delete;
    private:
        static void Initialize();
        static void Shutdown();
    private:
        inline static Vector<IImageLoader *> s_loaders;
    private:
        friend class Hyperion::AssetManager;
    };

}