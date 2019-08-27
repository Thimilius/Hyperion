#pragma once

#include "hyperion/common.hpp"

namespace Hyperion::Rendering {

    enum class ERenderTextureFormat {
        None,
        RGBA8,
        RGBA16F
    };

    enum class ERenderTextureTarget {
        None,
        DrawAndRead,
        Draw,
        Read
    };

    class CRenderTexture {
    protected:
        u32 m_width;
        u32 m_height;
        ERenderTextureFormat m_format;
    public:
        virtual ~CRenderTexture() { }

        inline u32 GetWidth() const { return m_width; }
        inline u32 GetHeight() const { return m_height; }
        inline ERenderTextureFormat GetFormat() const { return m_format; }

        virtual void Bind(ERenderTextureTarget target = ERenderTextureTarget::DrawAndRead) const = 0;
        virtual void Unbind() const = 0;

        virtual void Resize(u32 width, u32 height) = 0;

        static CRenderTexture *Create(u32 width, u32 height, ERenderTextureFormat format);
    };

}