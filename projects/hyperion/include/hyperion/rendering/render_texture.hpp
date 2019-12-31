#pragma once

#include "hyperion/common.hpp"
#include "hyperion/rendering/texture.hpp"

namespace Hyperion::Rendering {

    enum class RenderTextureFormat {
        RGBA8
    };

    class RenderTexture : public Texture {
    protected:
        u32 m_width;
        u32 m_height;
        RenderTextureFormat m_render_format;
    public:
        virtual ~RenderTexture() = default;

        inline u32 GetWidth() const { return m_width; }
        inline u32 GetHeight() const { return m_height; }
        inline RenderTextureFormat GetRenderFormat() const { return m_render_format; }

        virtual void BindTexture(u32 slot = 0) = 0;

        virtual void Resize(u32 width, u32 height) = 0;

        static Ref<RenderTexture> Create(u32 width, u32 height, RenderTextureFormat format);
    protected:
        static TextureFormat GetTextureFormat(RenderTextureFormat format);
    };

}