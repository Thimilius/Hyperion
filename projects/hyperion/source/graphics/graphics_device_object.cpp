//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/graphics/graphics_device_object.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Graphics {
    
    //--------------------------------------------------------------
    GraphicsDeviceObject::GraphicsDeviceObject(GraphicsDevice *device) {
        m_device = device;
    }

}