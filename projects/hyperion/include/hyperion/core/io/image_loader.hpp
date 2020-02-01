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
        // TODO: Support multiple loaders
        static Scope<ImageFormatLoader> m_loader;
    public:
        static const Vector<String> &GetSupportedExtensions();
        static bool SupportsExtension(const String &extension);

        static Ref<Image> Load(const String &path, bool flip_vertically = true);
    protected:
        ImageLoader() = default;
    };

}