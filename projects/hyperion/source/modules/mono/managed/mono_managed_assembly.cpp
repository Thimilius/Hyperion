//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/modules/mono/managed/mono_managed_assembly.hpp"

//---------------------- Library Includes ----------------------
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>

//-------------------- Definition Namespace --------------------
namespace Hyperion::Scripting {

    //--------------------------------------------------------------
    MonoManagedAssembly::MonoManagedAssembly(MonoAssembly *mono_assembly) {
        m_mono_assembly = mono_assembly;
        m_mono_assembly_image = mono_assembly_get_image(m_mono_assembly);

        HYP_ASSERT(m_mono_assembly_image);
    }

    //--------------------------------------------------------------
    MonoManagedMethod MonoManagedAssembly::FindMethod(const String &description) {
        MonoMethodDesc *method_description = mono_method_desc_new(description.c_str(), true);
        MonoMethod *mono_method = mono_method_desc_search_in_image(method_description, m_mono_assembly_image);
        if (mono_method == nullptr) {
            HYP_LOG_ERROR("Scripting", "Failed to find method: '{}'!", description);
        }
        mono_method_desc_free(method_description);
        return MonoManagedMethod(mono_method);
    }

    MonoClass *MonoManagedAssembly::FindClass(const String &name_space, const String &name) {
        MonoClass *mono_class = mono_class_from_name(m_mono_assembly_image, name_space.c_str(), name.c_str());
        if (mono_class == nullptr) {
            HYP_LOG_ERROR("Scripting", "Failed to find class: '{}.{}'!", name_space, name);
        }
        return mono_class;
    }

}