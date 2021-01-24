#include "hyppch.hpp"

#include "hyperion/assets/material.hpp"

#include "hyperion/rendering/render_driver.hpp"
#include "hyperion/rendering/render_engine.hpp"

using namespace Hyperion::Rendering;

namespace Hyperion {

    Material::Material(Shader *shader) {
        m_shader = shader;

        MaterialDescriptor descriptor;
        descriptor.shader = shader->GetResourceId();

        RenderEngine::GetRenderDriver()->CreateMaterial(m_resource_id, descriptor);
    }

    Material *Material::Create() {
        return new Material();
    }

    Material *Material::Create(Shader *shader) {
        return new Material(shader);
    }

    void Material::OnDestroy() {
        RenderEngine::GetRenderDriver()->DestroyMaterial(m_resource_id);
    }

}
