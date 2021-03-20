//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/modules/mono/managed/managed_assembly.hpp"

//---------------------- Library Includes ----------------------
#include <mono/metadata/assembly.h>

//-------------------- Definition Namespace --------------------
namespace Hyperion::Scripting {

    //--------------------------------------------------------------
    ManagedAssembly::ManagedAssembly(MonoAssembly *assembly) {
        m_assembly = assembly;
        m_image = mono_assembly_get_image(m_assembly);

        HYP_ASSERT(m_image);
    }

}