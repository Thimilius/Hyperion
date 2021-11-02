//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/assets/asset.hpp"
#include "hyperion/render/types/render_types_general.hpp"
#include "hyperion/render/types/render_types_texture.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    class Texture : public Asset {
    protected:
        Texture(AssetInfo info) : Asset(info) { }
    public:
        inline AssetType GetAssetType() const override { return AssetType::Texture; }

        virtual Rendering::TextureDimension GetDimension() const = 0;

        inline Rendering::TextureFormat GetFormat() const { return m_format; }
        inline const Rendering::TextureAttributes &GetAttributes() const { return m_attributes; }
    protected:
        Rendering::TextureFormat m_format;
        Rendering::TextureAttributes m_attributes;
    private:
        friend class Hyperion::AssetLoadSystem;
    };

    class Texture2D final : public Texture {
    private:
        Texture2D(AssetInfo info, const Rendering::Texture2DParameters &parameters);
        Texture2D(AssetInfo info, const Rendering::Texture2DParameters &parameters, const Rendering::TexturePixelData &pixels);
        ~Texture2D() = default;
    public:
        Rendering::TextureDimension GetDimension() const override { return Rendering::TextureDimension::Texture2D; }

        inline uint32 GetWidth() const { return m_width; }
        inline uint32 GetHeight() const { return m_height; }
        inline uint32 GetMipmapCount() const { return m_mipmap_count; }

        const Rendering::TexturePixelData &GetPixels() const;
        void SetPixels(const Rendering::TexturePixelData &data);
    private:
        void SetPixelsInternal(const Rendering::TexturePixelData &data);
    private:
        uint32 m_width;
        uint32 m_height;
        uint32 m_mipmap_count = 0;
        Rendering::TexturePixelData m_pixels;
    private:
        friend class Hyperion::AssetLoadSystem;
        friend class Hyperion::AssetManager;
    };

    class RenderTexture final : public Texture {
    private:
        RenderTexture(AssetInfo info, const Rendering::RenderTextureParameters &parameters);
        ~RenderTexture() = default;
    public:
        Rendering::TextureDimension GetDimension() const override { return Rendering::TextureDimension::RenderTexture; }
        
        inline uint32 GetWidth() const { return m_width; }
        inline uint32 GetHeight() const { return m_height; }
        inline uint32 GetMipmapCount() const { return m_mipmap_count; }
        inline const Array<Rendering::RenderTextureAttachment> GetAttachments() const { return m_attachments; }

        Rendering::RenderTargetId GetRenderTargetId() const;

        void Resize(uint32 width, uint32 height);
    private:
        uint32 m_width;
        uint32 m_height;
        uint32 m_mipmap_count = 0;
        Array<Rendering::RenderTextureAttachment> m_attachments;
    private:
        friend class Hyperion::AssetManager;
    };

}