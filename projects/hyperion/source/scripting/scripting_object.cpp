//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/scripting/scripting_object.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Scripting {

    //--------------------------------------------------------------
    ScriptingObject::~ScriptingObject() {
        ClearScriptingInstance();
    }

    //--------------------------------------------------------------
    void ScriptingObject::ClearScriptingInstance() {
        delete m_scripting_instance;
        m_scripting_instance = nullptr;
    }

    //--------------------------------------------------------------
    HYP_REFLECT_BEGIN(ScriptingObject)
    HYP_REFLECT_END()

}