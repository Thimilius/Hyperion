#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/assets/asset.hpp"
#include "hyperion/rendering/render_types.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    class Texture : public Asset {
    protected:
        Texture(AssetInfo info) : Asset(info) { }
    public:
        inline AssetType GetAssetType() const override { return AssetType::Texture; }

        virtual Graphics::GraphicsTextureDimension GetDimension() const = 0;

        inline Graphics::GraphicsTextureFormat GetFormat() const { return m_format; }
        inline const Rendering::TextureAttributes &GetAttributes() const { return m_attributes; }
    protected:
        Graphics::GraphicsTextureFormat m_format;
        Rendering::TextureAttributes m_attributes;
    };

    struct Texture2DCreationParameters {
        uint32 width;
        uint32 height;
        Graphics::GraphicsTextureFormat format;
        Rendering::TextureAttributes attributes;
    };

    class Texture2D final : public Texture {
    private:
        Texture2D(AssetInfo info) : Texture(info) { }
        Texture2D(AssetInfo info, Texture2DCreationParameters parameters, const Array<uint8> &pixels);
        ~Texture2D() = default;
    public:
        Graphics::GraphicsTextureDimension GetDimension() const override { return Graphics::GraphicsTextureDimension::Texture2D; }

        inline uint32 GetWidth() const { return m_width; }
        inline uint32 GetHeight() const { return m_height; }
    private:
        uint32 m_width;
        uint32 m_height;
        Array<byte> m_pixels;
    private:
        friend class Hyperion::AssetManager;
    };

}