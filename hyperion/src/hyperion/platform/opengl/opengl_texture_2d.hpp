#pragma once

#include "hyperion/rendering/texture.hpp"

namespace Hyperion::Rendering {

    class COpenGLTexture2D : public CTexture2D {
    private:
        u32 m_texture_id;
    public:
        COpenGLTexture2D(u32 width, u32 height, ETextureFormat format, ETextureWrapMode wrap_mode, ETextureFilter filter);
        COpenGLTexture2D(u32 width, u32 height, ETextureFormat format, ETextureWrapMode wrap_mode, ETextureFilter filter, const u8 *pixels);
        COpenGLTexture2D(const CString &path, ETextureWrapMode wrap_mode, ETextureFilter filter);
        ~COpenGLTexture2D();

        void Bind(u32 slot = 0) const override;
        void Unbind(u32 slot = 0) const override;

        void SetData(const void *pixels) override;

        void SetWrapMode(ETextureWrapMode wrap_mode) override;
        void SetFilter(ETextureFilter filter) override;
    private:
        void CreateTexture(const u8 *pixels);

        u32 GetGLFormat() const;
        u32 GetGLWrapMode() const;
    };

}