#pragma once

#include "hyperion/common.hpp"

namespace Hyperion::Rendering {

    enum class RenderTextureFormat {
        RGBA8,
        RGBA16F
    };

    class RenderTexture {
    protected:
        u32 m_width;
        u32 m_height;
        RenderTextureFormat m_format;
    public:
        virtual ~RenderTexture() { }

        inline u32 GetWidth() const { return m_width; }
        inline u32 GetHeight() const { return m_height; }
        inline RenderTextureFormat GetFormat() const { return m_format; }

        virtual u32 GetID() const = 0;

        virtual void BindTexture(u32 slot = 0) = 0;

        virtual void Resize(u32 width, u32 height) = 0;

        static Ref<RenderTexture> Create(u32 width, u32 height, RenderTextureFormat format);
    };

}