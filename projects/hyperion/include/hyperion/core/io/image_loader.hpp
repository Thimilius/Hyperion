#pragma once

#include "hyperion/core/image.hpp"

namespace Hyperion {

    class ImageFormatLoader {
    public:
        virtual ~ImageFormatLoader() = default;

        virtual const Vector<String> &GetSupportedExtensions() const = 0;
        virtual bool SupportsExtension(const String &extension) const = 0;

        virtual Ref<Image> Load(const String &path, bool flip_vertically) = 0;
    };

    class ImageLoader {
    private:
        inline static Vector<ImageFormatLoader*> s_loaders;
    public:
        static Vector<String> GetSupportedExtensions();
        static bool SupportsExtension(const String &extension);

        static Ref<Image> Load(const String &path, bool flip_vertically = true);
        static void AddFormatLoader(ImageFormatLoader *image_format_loader);
    private:
        ImageLoader() = delete;
        ~ImageLoader() = delete;

        static void Init();
        static void Shutdown();

        friend class AssetManager;
    };

}