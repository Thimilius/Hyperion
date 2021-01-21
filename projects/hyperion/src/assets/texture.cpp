#include "hyppch.hpp"

#include "hyperion/assets/texture.hpp"

#include "hyperion/rendering/render_engine.hpp"
#include "hyperion/rendering/render_driver.hpp"

using namespace Hyperion::Rendering;

namespace Hyperion {

    uint32 Texture::CalculateMipmapCount(uint32 width, uint32 height) {
        return static_cast<uint32>(1 + Math::Floor(Math::Log2(Math::Max(static_cast<float32>(width), static_cast<float32>(height)))));
    }

    Texture2D::Texture2D(uint32 width, uint32 height, TextureFormat format, TextureParameters parameters, const Vector<uint8> &pixels, bool read_and_write_enabled) {
        m_width = width;
        m_height = height;
        m_parameters = parameters;
        m_read_and_write_enabled = read_and_write_enabled;

        if (read_and_write_enabled) {
            m_pixels = pixels;
        }

        TextureDescriptor descriptor;
        descriptor.dimension = GetDimension();
        descriptor.format = format;
        descriptor.parameters = parameters;
        descriptor.size = { width, height };
        descriptor.mipmap_count = CalculateMipmapCount(width, height);
        descriptor.pixels = pixels;

        RenderEngine::GetRenderDriver()->CreateTexture(m_resource_id, descriptor);
    }

    Texture::~Texture() {
        RenderEngine::GetRenderDriver()->FreeTexture(m_resource_id);
    }

    Texture2D *Texture2D::Create(uint32 width, uint32 height, TextureFormat format, TextureParameters parameters, const Vector<uint8> &pixels, bool read_and_write_enabled) {
        return new Texture2D(width, height, format, parameters, pixels, read_and_write_enabled);
    }

}