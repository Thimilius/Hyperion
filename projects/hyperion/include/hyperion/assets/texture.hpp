#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/assets/asset.hpp"
#include "hyperion/core/image.hpp"
#include "hyperion/rendering/attributes/texture_attributes.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    class Texture : public Asset {
        HYP_REFLECT(Asset);
    public:
        inline AssetType GetAssetType() const override { return AssetType::Texture; }

        virtual Rendering::TextureDimension GetDimension() const = 0;

        inline bool IsReadAndWriteEnabled() const { return m_read_and_write_enabled; }
        inline const Rendering::TextureParameters &GetParamters() const { return m_parameters; }
    protected:
        virtual void OnDestroy() override;
    protected:
        static uint32 CalculateMipmapCount(uint32 width, uint32 height);
    protected:
        bool m_read_and_write_enabled;
        Rendering::TextureParameters m_parameters;
    };

    class Texture2D final : public Texture {
        HYP_REFLECT(Texture);
    public:
        inline Rendering::TextureDimension GetDimension() const override { return Rendering::TextureDimension::Texture2D; }

        inline uint32 GetWidth() const { return m_width; }
        inline uint32 GetHeight() const { return m_height; }
    public:
        static Texture2D *Create(uint32 width, uint32 height, Rendering::TextureFormat format, Rendering::TextureParameters parameters, const Vector<uint8> &pixels, bool read_and_write_enabled = false);
    private:
        Texture2D() = default;
        Texture2D(uint32 width, uint32 height, Rendering::TextureFormat format, Rendering::TextureParameters parameters, const Vector<uint8> &pixels, bool read_and_write_enabled);
    private:
        static Texture2D *Create();
    private:
        uint32 m_width;
        uint32 m_height;
        Vector<uint8> m_pixels;
    };

    class TextureCubemap final : public Texture {
        HYP_REFLECT(Texture);
    public:
        inline Rendering::TextureDimension GetDimension() const override { return Rendering::TextureDimension::TextureCubemap; }
    private:
        TextureCubemap() = default;
    private:
        static TextureCubemap *Create();
    };

    class RenderTexture final : public Texture {
        HYP_REFLECT(Texture);
    public:
        inline AssetType GetAssetType() const override { return AssetType::RenderTexture; }

        inline Rendering::TextureDimension GetDimension() const override { return Rendering::TextureDimension::Texture2D; }

        inline uint32 GetWidth() const { return m_width; }
        inline uint32 GetHeight() const { return m_height; }

        void Resize(uint32 width, uint32 height);
    public:
        static RenderTexture *Create(uint32 width, uint32 height, Vector<Rendering::RenderTextureAttachment> attachments);
    protected:
        void OnDestroy() override;
    private:
        RenderTexture() = default;
        RenderTexture(uint32 width, uint32 height, Vector<Rendering::RenderTextureAttachment> attachments);
    private:
        static RenderTexture *Create();
    private:
        uint32 m_width;
        uint32 m_height;
        Vector<Rendering::RenderTextureAttachment> m_attachments;
    };

}