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
  void Shader::Recompile(const String &source) {
    PreProcess(source);
  }

  //--------------------------------------------------------------
  Shader::Shader(AssetInfo info, const String &source) : Asset(info) {
    Recompile(source);
  }

  //--------------------------------------------------------------
  void Shader::PreProcess(const String &source) {
    ShaderPreProcessor pre_processor(source);
    ShaderPreProcessResult pre_process_result = pre_processor.PreProcess();
    if (pre_process_result.success) {
      m_data = std::move(pre_process_result.data);
      m_is_valid = true;
    } else {
      m_is_valid = false;
    }

    SetDirty();
  }

}