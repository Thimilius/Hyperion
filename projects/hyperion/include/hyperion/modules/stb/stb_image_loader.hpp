#pragma once

#include "hyperion/core/io/image_loader.hpp"

namespace Hyperion {

    class StbImageLoader : public ImageLoader {
    public:
        Ref<Image> LoadFromFile(const String &path, bool flip_vertically) override;
    };

}