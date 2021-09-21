//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/assets/shader.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/render/shader/shader_pre_processor.hpp"

//------------------------- Namespaces -------------------------
using namespace Hyperion::Rendering;

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    //--------------------------------------------------------------
    Shader::Shader(AssetInfo info, const String &source) : Asset(info) {
        PreProcess(source);
    }

    //--------------------------------------------------------------
    void Shader::PreProcess(const String &source) {
        ShaderPreProcessor pre_processor(source);
        ShaderPreProcessResult pre_process_result = pre_processor.PreProcess();
        if (pre_process_result.success) {
            m_data = pre_process_result.data;
        }

        // TEMP: This will get extracted by the pre processor.
        MaterialProperty property;
        property.id = MaterialProperty::NameToId("u_color");
        property.type = MaterialPropertyType::Color;
        property.storage.color = Color::White();
        m_default_properties.Add(property);

        SetDirty();
    }

}