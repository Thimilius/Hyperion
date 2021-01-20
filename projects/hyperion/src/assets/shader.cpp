#include "hyppch.hpp"

#include "hyperion/assets/shader.hpp"

#include "hyperion/rendering/render_engine.hpp"
#include "hyperion/rendering/render_driver.hpp"

using namespace Hyperion::Rendering;

namespace Hyperion {

    Shader::Shader(const Map<ShaderStageFlags, String> &sources) {
        ShaderDescriptor descriptor;
        for (auto &[stage_flags, source] : sources) {
            descriptor.stage_flags |= stage_flags;

            if ((stage_flags & ShaderStageFlags::Vertex) == ShaderStageFlags::Vertex) {
                descriptor.source_vertex = source;
            } else if ((stage_flags & ShaderStageFlags::Fragment) == ShaderStageFlags::Fragment) {
                descriptor.source_fragment = source;
            }
        }

        Rendering::RenderEngine::GetRenderDriver()->CreateShader(m_resource_id, descriptor);
    }

    Shader::~Shader() {
        Rendering::RenderEngine::GetRenderDriver()->FreeShader(m_resource_id);
    }

    Shader *Shader::Create(const Map<ShaderStageFlags, String> &sources) {
        return new Shader(sources);
    }

}