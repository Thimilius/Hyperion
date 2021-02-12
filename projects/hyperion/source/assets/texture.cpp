//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/assets/texture.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/rendering/render_driver.hpp"
#include "hyperion/rendering/render_engine.hpp"

//------------------------- Namespaces -------------------------
using namespace Hyperion::Rendering;

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    //--------------------------------------------------------------
    void Texture::OnDestroy() {
        RenderEngine::GetRenderDriver()->DestroyTexture(m_resource_id);
    }

    //--------------------------------------------------------------
    uint32 Texture::CalculateMipmapCount(uint32 width, uint32 height) {
        return static_cast<uint32>(1 + Math::Floor(Math::Log2(Math::Max(static_cast<float32>(width), static_cast<float32>(height)))));
    }

    //--------------------------------------------------------------
    Texture2D::Texture2D(uint32 width, uint32 height, TextureFormat format, TextureParameters parameters, const Vector<uint8> &pixels, bool read_and_write_enabled) {
        m_width = width;
        m_height = height;
        m_parameters = parameters;
        m_read_and_write_enabled = read_and_write_enabled;

        if (read_and_write_enabled) {
            m_pixels = pixels;
        }

        TextureDescriptor descriptor = { };
        descriptor.dimension = GetDimension();
        descriptor.format = format;
        descriptor.parameters = parameters;
        descriptor.size = { width, height };
        descriptor.mipmap_count = CalculateMipmapCount(width, height);
        descriptor.pixels = pixels;

        RenderEngine::GetRenderDriver()->CreateTexture(m_resource_id, descriptor);
    }

    //--------------------------------------------------------------
    Texture2D *Texture2D::Create() {
        return new Texture2D();
    }

    //--------------------------------------------------------------
    Texture2D *Texture2D::Create(uint32 width, uint32 height, TextureFormat format, TextureParameters parameters, const Vector<uint8> &pixels, bool read_and_write_enabled) {
        return new Texture2D(width, height, format, parameters, pixels, read_and_write_enabled);
    }

    //--------------------------------------------------------------
    TextureCubemap *TextureCubemap::Create() {
        return new TextureCubemap();
    }

    //--------------------------------------------------------------
    void RenderTexture::OnDestroy() {
        RenderEngine::GetRenderDriver()->DestroyRenderTexture(m_resource_id);
    }

    //--------------------------------------------------------------
    RenderTexture::RenderTexture(uint32 width, uint32 height, Vector<RenderTextureAttachment> attachments) {
        m_width = width;
        m_height = height;
        m_attachments = attachments;

        RenderTextureDescriptor descriptor = { };
        descriptor.size = { width, height };
        descriptor.mipmap_count = CalculateMipmapCount(width, height);
        descriptor.attachments = attachments;

        RenderEngine::GetRenderDriver()->CreateRenderTexture(m_resource_id, descriptor);
    }

    //--------------------------------------------------------------
    void RenderTexture::Resize(uint32 width, uint32 height) {
        m_width = width;
        m_height = height;

        RenderEngine::GetRenderDriver()->ResizeRenderTexture(m_resource_id, width, height, CalculateMipmapCount(width, height));
    }

    //--------------------------------------------------------------
    RenderTexture *RenderTexture::Create(uint32 width, uint32 height, Vector<RenderTextureAttachment> attachments) {
        return new RenderTexture(width, height, attachments);
    }

    //--------------------------------------------------------------
    RenderTexture *RenderTexture::Create() {
        return new RenderTexture();
    }

}
