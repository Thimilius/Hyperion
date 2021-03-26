//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/modules/mono/mono_scripting_instance.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/core/app/time.hpp"
#include "hyperion/core/system/engine.hpp"
#include "hyperion/modules/mono/mono_scripting_driver.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Scripting {

    //--------------------------------------------------------------
    MonoScriptingType::MonoScriptingType(MonoClass *mono_class) {
        m_mono_class = mono_class;
    }

    //--------------------------------------------------------------
    MonoScriptingInstanceBase::MonoScriptingInstanceBase(MonoObject *mono_object) {
        m_mono_object = mono_object;
    }

    //--------------------------------------------------------------
    ScriptingType *MonoScriptingInstanceBase::GetScriptingType() const {
        return nullptr;
    }

    //--------------------------------------------------------------
    void MonoScriptingInstance::SendMessage(ScriptingMessage message) {
        switch (message) {
            case ScriptingMessage::OnCreate: {
                break;
            }
            case ScriptingMessage::OnUpdate: {
                break;
            }
            case ScriptingMessage::OnDestroy: {
                break;
            }
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE;
        }
    }

}