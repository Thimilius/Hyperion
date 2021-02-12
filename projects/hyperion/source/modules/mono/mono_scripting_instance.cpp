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
    MonoScriptingInstance::MonoScriptingInstance(MonoObject *managed_object, bool is_script_component) {
        m_managed_object = managed_object;
        m_is_script_component = is_script_component;
    }

    //--------------------------------------------------------------
    void MonoScriptingInstance::SendMessage(ScriptingMessage message) {
        // Only script components get all scripting messages.
        if (m_is_script_component) {
            switch (message) {
                case ScriptingMessage::OnCreate: {
                    CallMethod("OnCreate", 0, nullptr);
                    break;
                }
                case ScriptingMessage::OnUpdate: {
                    float32 delta_time = Time::GetDeltaTime();
                    void *args[1] = { &delta_time };
                    CallMethod("OnUpdate", 1, args);
                    break;
                }
                case ScriptingMessage::OnDestroy: {
                    CallMethod("OnDestroy", 0, nullptr);
                    break;
                }
                default: HYP_ASSERT_ENUM_OUT_OF_RANGE;
            }
        }

        // The OnDestroy message gets processed by every scripting instance for cleanup.
        if (message == ScriptingMessage::OnDestroy) {
            MonoScriptingDriver::UnregisterObject(m_managed_object, m_is_script_component);
            delete this; // I don't really know how I feel about this...
        }
    }

    //--------------------------------------------------------------
    void MonoScriptingInstance::CallMethod(const char *name, int32 parameter_count, void **args) {
        if (Engine::GetMode() == EngineMode::EditorRuntime) {
            MonoClass *script_class = MonoScriptingDriver::GetScriptClass();
            MonoMethod *method = mono_class_get_method_from_name(script_class, name, parameter_count);
            MonoMethod *virtual_method = mono_object_get_virtual_method(m_managed_object, method);
            MonoObject *exception = nullptr;
            mono_runtime_invoke(virtual_method, m_managed_object, args, &exception);
        }
    }

}