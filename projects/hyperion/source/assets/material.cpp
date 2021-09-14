#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/assets/material.hpp"

//------------------------- Namespaces -------------------------
using namespace Hyperion::Rendering;

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    //--------------------------------------------------------------
    Material::Material(AssetInfo info, Shader *shader) : Asset(info) {
        m_shader = shader;

        Threading::ScopeLock lock(GetLocker());
        m_properties = shader->GetDefaultProperties();

        SetDirty();
    }

}