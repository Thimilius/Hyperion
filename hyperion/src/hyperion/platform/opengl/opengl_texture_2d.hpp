#pragma once

#include "hyperion/rendering/texture.hpp"

namespace Hyperion::Rendering {

    class COpenGLTexture2D : public CTexture2D {
    private:
        u32 m_texture_id;
    public:
        COpenGLTexture2D(u32 width, u32 height, ETextureFormat mode, ETextureWrapMode wrap_mode, ETextureFilter filter);
        COpenGLTexture2D(u32 width, u32 height, ETextureFormat mode, ETextureWrapMode wrap_mode, ETextureFilter filter, const void *pixels);
        ~COpenGLTexture2D();

        void Bind(u32 slot = 0) const override;
        void Unbind(u32 slot = 0) const override;

        void SetData(const void *pixels) override;

        void SetWrapMode(ETextureWrapMode wrap_mode) override;
        void SetFilter(ETextureFilter filter) override;
    private:
        u32 GetGLFormat() const;
        u32 GetGLWrapMode() const;
    };

}