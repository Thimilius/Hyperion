#include "hyppch.hpp"

#include "hyperion/assets/shader.hpp"

#include "hyperion/rendering/render_driver.hpp"
#include "hyperion/rendering/render_engine.hpp"

using namespace Hyperion::Rendering;

namespace Hyperion {

    Shader::Shader(const String &source) {
        ShaderDescriptor descriptor = { };
        descriptor.source = source;
        Rendering::RenderEngine::GetRenderDriver()->CreateShader(m_resource_id, descriptor);
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
