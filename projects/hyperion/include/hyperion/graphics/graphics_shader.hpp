#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/graphics/graphics_device_object.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Graphics {

    enum class GraphicsShaderType {
        Unknown,

        Vertex = BIT(0),
        Pixel  = BIT(1),
    };
    HYP_CREATE_ENUM_FLAG_OPERATORS(GraphicsShaderType);

    struct GraphicsShaderDescription : public GraphicsDeviceObjectDescription {
        GraphicsShaderType type;

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