#include "hyppch.hpp"

#include "hyperion/assets/shader.hpp"

#include "hyperion/rendering/render_driver.hpp"
#include "hyperion/rendering/render_engine.hpp"
#include "hyperion/rendering/shaders/shader_pre_processor.hpp"

using namespace Hyperion::Rendering;

namespace Hyperion {

    Shader::Shader(const String &source) {
        ShaderPreProcessor pre_processor(source);
        ShaderPreProcessResult pre_process_result = pre_processor.PreProcess();

        ShaderDescriptor descriptor = { };
        if (pre_process_result.success) {
            ShaderStageFlags stage_flags = pre_process_result.stage_flags;
            Map<ShaderStageFlags, String> &sources = pre_process_result.sources;

            descriptor.use_fallback = false;
            descriptor.stage_flags = pre_process_result.stage_flags;
            if ((stage_flags & ShaderStageFlags::Vertex) == ShaderStageFlags::Vertex) {
                descriptor.source_vertex = sources[ShaderStageFlags::Vertex];
            }
            if ((stage_flags & ShaderStageFlags::Fragment) == ShaderStageFlags::Fragment) {
                descriptor.source_fragment = sources[ShaderStageFlags::Fragment];
            }
            
            Rendering::RenderEngine::GetRenderDriver()->CreateShader(m_resource_id, descriptor);
        } else {
            descriptor.use_fallback = true;

            Rendering::RenderEngine::GetRenderDriver()->CreateShader(m_resource_id, descriptor);
        }
    }

    Shader *Shader::Create() {
        return new Shader();
    }

    Shader *Shader::Create(const String &source) {
        return new Shader(source);
    }

    void Shader::OnDestroy() {
        Rendering::RenderEngine::GetRenderDriver()->DestroyShader(m_resource_id);
    }

}
