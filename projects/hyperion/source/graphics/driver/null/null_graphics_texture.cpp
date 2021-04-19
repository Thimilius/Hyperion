//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/graphics/driver/null/null_graphics_texture.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Graphics {

    //--------------------------------------------------------------
    NullGraphicsTexture::NullGraphicsTexture(GraphicsDevice *device, const GraphicsTextureDescription &description) : GraphicsTexture(device, description) {

    }

}