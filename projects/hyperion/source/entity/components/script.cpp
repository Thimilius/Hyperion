//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/entity/components/script.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    //--------------------------------------------------------------
    HYP_REFLECT_BEGIN(Script)
    HYP_REFLECT_BASE(Component)
    HYP_REFLECT_CONSTRUCTOR([]() { return new Script(); })
    HYP_REFLECT_END()

}