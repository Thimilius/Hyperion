//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/graphics/graphics_shader.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Graphics {

    //--------------------------------------------------------------
    GraphicsShader::GraphicsShader(GraphicsDevice *device, const GraphicsShaderDescription &description) : GraphicsDeviceObject(device) {
        m_description = description;
    }

}