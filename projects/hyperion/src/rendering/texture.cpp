#include "hyppch.hpp"

#include "hyperion/rendering/texture.hpp"

namespace Hyperion::Rendering {

    u32 Texture::GetBytesPerPixel(TextureFormat format) {
        switch (format) {
            case Hyperion::Rendering::TextureFormat::RGB24: return 3;
            case Hyperion::Rendering::TextureFormat::RGBA32: return 4;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return 0;
        }
    }

    u32 Texture::CalculateMipmapCount(u32 width, u32 height) {
        return (u32)(1 + Math::Floor(Math::Log2(Math::Max((f32)width, (f32)height))));
    }

}