#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/graphics/graphics_device_object.hpp"
#include "hyperion/graphics/graphics_types.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Graphics {

    struct GraphicsShaderDescription : public GraphicsDeviceObjectDescription {
        GraphicsShaderStageFlags stage;

        uint64 byte_code_size = 0;
        const void *byte_code = nullptr;

        const char *entry_point = "main";
    };

    class GraphicsShader : public GraphicsDeviceObject {
    public:
        virtual ~GraphicsShader() = default;
    protected:
        GraphicsShader(GraphicsDevice *device, const GraphicsShaderDescription &description);
    public:
        virtual const GraphicsShaderDescription &GetDescription() const override { return m_description; }
    protected:
        GraphicsShaderDescription m_description;
    };

}