//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/scripting/scripting_object.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Scripting {

    //--------------------------------------------------------------
    ScriptingObject::~ScriptingObject() {
        delete m_scripting_instance;
    }

}