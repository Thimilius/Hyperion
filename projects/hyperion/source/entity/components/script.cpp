//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/entity/components/script.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/entity/entity.hpp"

//------------------------- Namespaces -------------------------
using namespace Hyperion::Scripting;

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    //--------------------------------------------------------------
    Script *Script::Create() {
        return new Script();
    }

}