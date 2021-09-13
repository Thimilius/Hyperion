//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/assets/texture.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    //--------------------------------------------------------------
    Texture2D::Texture2D(AssetInfo info, Texture2DCreationParameters parameters, const Array<uint8> &pixels) : Texture2D(info) {
        m_width = parameters.width;
        m_height = parameters.height;
        m_format = parameters.format;
        m_attributes = parameters.attributes;
        m_pixels = pixels;
    }

}