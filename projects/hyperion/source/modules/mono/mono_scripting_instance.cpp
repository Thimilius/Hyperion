//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/modules/mono/mono_scripting_instance.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/core/app/time.hpp"
#include "hyperion/core/system/engine.hpp"
#include "hyperion/modules/mono/mono_scripting_driver.hpp"
#include "hyperion/modules/mono/mono_scripting_storage.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Scripting {

    //--------------------------------------------------------------
    MonoScriptingType::MonoScriptingType(MonoClass *mono_class) {
        m_mono_class = mono_class;
    }

    //--------------------------------------------------------------
    MonoScriptingInstance::MonoScriptingInstance(MonoObject *mono_object) {
        m_mono_object = mono_object;
        // NOTE: For now every object is going to be pinned.
        // This may be bad for the memory footprint but would require a more dynamic handling of objects in the scripting storage.
        m_gc_handle = mono_gchandle_new_v2(mono_object, true);
    }

    //--------------------------------------------------------------
    MonoScriptingInstance::~MonoScriptingInstance() {
        mono_gchandle_free_v2(m_gc_handle);
        MonoScriptingStorage::UnregisterMonoObject(m_mono_object);
    }

    //--------------------------------------------------------------
    ScriptingType *MonoScriptingInstance::GetScriptingType() const {
        return MonoScriptingStorage::GetOrCreateScriptingType(mono_object_get_class(m_mono_object));
    }

    //--------------------------------------------------------------
    MonoScriptingInstanceScript::MonoScriptingInstanceScript(MonoObject *mono_object) : MonoScriptingInstance(mono_object) { }

    //--------------------------------------------------------------
    void MonoScriptingInstanceScript::SendMessage(ScriptingMessage message) {
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