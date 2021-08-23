#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/image.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    class IImageLoader {
    public:
        virtual ~IImageLoader() = default;
    public:
        virtual const List<String> &GetSupportedExtensions() const = 0;
        virtual bool SupportsExtension(const String &extension) const = 0;

        virtual Image *Load(const String &path, bool flip_vertically) = 0;
    };

    class ImageLoader final {
    public:
        static List<String> GetSupportedExtensions();
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
        inline static List<IImageLoader *> s_loaders;
    };

}