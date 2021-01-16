#include "hyppch.hpp"

#include "hyperion/assets/shader.hpp"

#include "hyperion/rendering/render_engine.hpp"
#include "hyperion/rendering/render_driver.hpp"

namespace Hyperion {

    Shader::Shader(const String &vertex, const String &fragment) {
        Rendering::ShaderDescriptor descriptor;
        descriptor.stage_flags = Rendering::ShaderStageFlags::Vertex | Rendering::ShaderStageFlags::Fragment;
        descriptor.vertex = vertex;
        descriptor.fragment = fragment;
        Rendering::RenderEngine::GetRenderDriver()->CreateShader(m_resource_id, descriptor);
    }

    Shader::~Shader() {
        Rendering::RenderEngine::GetRenderDriver()->FreeShader(m_resource_id);
    }

    Shader *Shader::Create(const String &vertex, const String &fragment) {
        return new Shader(vertex, fragment);
    }

}