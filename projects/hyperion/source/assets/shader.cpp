//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/assets/shader.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/rendering/render_driver.hpp"
#include "hyperion/rendering/render_engine.hpp"
#include "hyperion/rendering/shaders/shader_pre_processor.hpp"

//------------------------- Namespaces -------------------------
using namespace Hyperion::Rendering;

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    //--------------------------------------------------------------
    Shader::Shader(const String &source) {
        ResourceId resource_id = m_resource_id;
        PreProcess(source, [resource_id](const ShaderDescriptor &descriptor) {
            RenderEngine::GetRenderDriver()->CreateShader(resource_id, descriptor);
        });
    }

    //--------------------------------------------------------------
    Shader *Shader::Create() {
        return new Shader();
    }

    //--------------------------------------------------------------
    void Shader::RegisterRecompilationListener(IShaderRecompilationListener *recompilation_listener) {
        HYP_ASSERT(std::find(m_recompilation_listeners.begin(), m_recompilation_listeners.end(), recompilation_listener) == m_recompilation_listeners.end());

        m_recompilation_listeners.push_back(recompilation_listener);
    }

    //--------------------------------------------------------------
    void Shader::UnregisterRecompilationListener(IShaderRecompilationListener *recompilation_listener) {
        auto begin = m_recompilation_listeners.begin();
        auto end = m_recompilation_listeners.end();
        if (std::find(begin, end, recompilation_listener) != end) {
            m_recompilation_listeners.erase(std::remove(begin, end, recompilation_listener));
        }
    }

    //--------------------------------------------------------------
    void Shader::Recompile(const String &source) {
        ResourceId resource_id = m_resource_id;
        PreProcess(source, [resource_id](const ShaderDescriptor &descriptor) {
            RenderEngine::GetRenderDriver()->RecompileShader(resource_id, descriptor);
        });

        for (IShaderRecompilationListener *recompilation_listener : m_recompilation_listeners) {
            recompilation_listener->OnRecompile();
        }
    }

    //--------------------------------------------------------------
    void Shader::PreProcess(const String &source, ShaderPreProcessCallback callback) {
        ShaderPreProcessor pre_processor(source);
        ShaderPreProcessResult pre_process_result = pre_processor.PreProcess();

        ShaderDescriptor descriptor = { };
        if (pre_process_result.success) {
            m_attributes = pre_process_result.attributes;

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
        } else {
            m_attributes = ShaderAttributes();

            descriptor.use_fallback = true;
        }

        callback(descriptor);
    }

    //--------------------------------------------------------------
    Shader *Shader::Create(const String &source) {
        return new Shader(source);
    }

    //--------------------------------------------------------------
    void Shader::OnDestroy() {
        RenderEngine::GetRenderDriver()->DestroyShader(m_resource_id);
    }

}
