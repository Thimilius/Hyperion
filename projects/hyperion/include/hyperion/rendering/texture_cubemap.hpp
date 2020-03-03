#pragma once

#include "hyperion/rendering/texture.hpp"

namespace Hyperion::Rendering {

    enum class TextureCubemapFace {
        PositiveX, // Right
        NegativeX, // Left
        PositiveY, // Top
        NegativeY, // Bottom
        PositiveZ, // Back
        NegativeZ  // Front
    };

    class TextureCubemap : public Texture {
    protected:
        u32 m_width;
        u32 m_height;
    public:
        virtual ~TextureCubemap() = default;
        
        inline TextureDimension GetDimension() const override { return TextureDimension::Texture2D; }

        inline u32 GetWidth() const { return m_width; }
        inline u32 GetHeight() const { return m_height; }

        virtual void Bind(u32 slot = 0) const = 0;
        virtual void Unbind(u32 slot = 0) const = 0;

        virtual void SetPixels(TextureCubemapFace face, const u8 *pixels, bool generate_mipmaps = true) = 0;

        static Ref<TextureCubemap> Create(u32 width, u32 height, TextureFormat format, TextureParameters parameters);
        static Ref<TextureCubemap> Create(u32 width, u32 height, TextureFormat format, TextureParameters parameters, const Map<TextureCubemapFace, const u8 *> &pixels);
    };

}