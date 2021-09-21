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
            m_data = pre_process_result.shader_data;
            m_default_properties = std::move(pre_process_result.properties);

            SetDirty();
        } else {
            // TODO: Use fallback shader.
        }
    }

}