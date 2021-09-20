//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/assets/texture.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    //--------------------------------------------------------------
    Texture2D::Texture2D(AssetInfo info, const Rendering::Texture2DParameters &parameters) : Texture(info) {
        m_width = parameters.width;
        m_height = parameters.height;
        m_format = parameters.format;
        m_attributes = parameters.attributes;
    }

    //--------------------------------------------------------------
    Texture2D::Texture2D(AssetInfo info, const Rendering::Texture2DParameters &parameters, const Rendering::TexturePixelData &pixels) : Texture2D(info, parameters) {
        SetPixelsInternal(pixels);
    }

    //--------------------------------------------------------------
    const Rendering::TexturePixelData &Texture2D::GetPixels() const {
        ValidateDataAccess();
        return m_pixels;
    }

    //--------------------------------------------------------------
    void Texture2D::SetPixels(const Rendering::TexturePixelData &pixels) {
        if (!ValidateDataAccess()) {
            return;
        }

        SetPixelsInternal(pixels);
    }

    //--------------------------------------------------------------
    void Texture2D::SetPixelsInternal(const Rendering::TexturePixelData &data) {
        m_pixels = data;

        SetDirty();
    }

}