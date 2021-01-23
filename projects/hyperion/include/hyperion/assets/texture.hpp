#pragma once

#include "hyperion/assets/asset.hpp"
#include "hyperion/core/image.hpp"
#include "hyperion/rendering/attributes/texture_attributes.hpp"

namespace Hyperion {

    class Texture : public Asset {
        HYP_REFLECT(Asset);
    public:
        virtual ~Texture() override;

        inline AssetType GetAssetType() const override { return AssetType::Texture; }

        virtual Rendering::TextureDimension GetDimension() const = 0;
    protected:
        static uint32 CalculateMipmapCount(uint32 width, uint32 height);
    protected:
        bool m_read_and_write_enabled;
    };

    class Texture2D : public Texture {
        HYP_REFLECT(Texture);
    public:
        inline Rendering::TextureDimension GetDimension() const { return Rendering::TextureDimension::Texture2D; }

        inline uint32 GetWidth() const { return m_width; }
        inline uint32 GetHeight() const { return m_height; }
        inline const Rendering::TextureParameters &GetParamters() const { return m_parameters; }

        static Texture2D *Create(uint32 width, uint32 height, Rendering::TextureFormat format, Rendering::TextureParameters parameters, const Vector<uint8> &pixels, bool read_and_write_enabled = false);
    private:
        Texture2D(uint32 width, uint32 height, Rendering::TextureFormat format, Rendering::TextureParameters parameters, const Vector<uint8> &pixels, bool read_and_write_enabled);
    private:
        uint32 m_width;
        uint32 m_height;
        Rendering::TextureParameters m_parameters;
        Vector<uint8> m_pixels;
    };

    class TextureCubemap : public Texture {
        HYP_REFLECT(Texture);
    public:
        inline Rendering::TextureDimension GetDimension() const { return Rendering::TextureDimension::TextureCubemap; }
    };

}