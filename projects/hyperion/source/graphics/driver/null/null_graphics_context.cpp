//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/graphics/driver/null/null_graphics_context.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Graphics {

    //--------------------------------------------------------------
    GraphicsContextProperties NullGraphicsContext::GetProperties() const {
        GraphicsContextProperties properties;
        properties.renderer = "Null";
        properties.vendor = "Null";
        properties.version = "1.0.0";
        return properties;
    }

    //--------------------------------------------------------------
    void NullGraphicsContext::Initialize(const GraphicsContextDescriptor &descriptor) {
        HYP_LOG_INFO("Graphics", "Initialized Null graphics driver!");
    }

    //--------------------------------------------------------------
    void NullGraphicsContext::Shutdown() {

    }

}