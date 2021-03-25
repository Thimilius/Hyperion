//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/modules/mono/managed/mono_managed_exception.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/modules/mono/managed/mono_managed_string.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Scripting {

    //--------------------------------------------------------------
    MonoManagedException::MonoManagedException(MonoObject *mono_exception) : MonoManagedException(reinterpret_cast<MonoException *>(mono_exception)) { }

    //--------------------------------------------------------------
    MonoManagedException::MonoManagedException(MonoException *mono_exception) {
        m_mono_exception = mono_exception;
    }

    //--------------------------------------------------------------
    void MonoManagedException::Print() {
        MonoClass *exception_class = mono_object_get_class(reinterpret_cast<MonoObject *>(m_mono_exception));

        MonoProperty *message_property = mono_class_get_property_from_name(mono_get_exception_class(), "Message");
        MonoObject *managed_message = mono_property_get_value(message_property, m_mono_exception, nullptr, nullptr);
        String message = MonoManagedString(reinterpret_cast<MonoString *>(managed_message)).GetString();

        MonoProperty *stack_trace_property = mono_class_get_property_from_name(mono_get_exception_class(), "StackTrace");
        MonoObject *managed_stack_trace = mono_property_get_value(stack_trace_property, m_mono_exception, nullptr, nullptr);
        String stack_trace = managed_stack_trace != nullptr ? MonoManagedString(reinterpret_cast<MonoString *>(managed_stack_trace)).GetString() : "";

        HYP_LOG_ERROR("Scripting", "{}: {}\n{}", mono_class_get_name(exception_class), message, stack_trace);
    }

}