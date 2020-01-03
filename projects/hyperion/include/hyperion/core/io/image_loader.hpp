#pragma once

#include "hyperion/core/image.hpp"

namespace Hyperion {

    class ImageLoader {
    private:
        static Scope<ImageLoader> m_loader;
    public:
        virtual ~ImageLoader() = default;

        virtual Ref<Image> LoadFromFile(const String &path, bool flip_vertically) = 0;

        static Ref<Image> Load(const String &path, bool flip_vertically = true);
    protected:
        ImageLoader() = default;
    };

}