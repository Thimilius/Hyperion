//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/entity/components/rendering/light.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/entity/world.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    //--------------------------------------------------------------
    void Light::OnCreate() {
        Behaviour::OnCreate();

        GetWorld()->AddLight(this);
    }

    //--------------------------------------------------------------
    void Light::OnDestroy() {
        GetWorld()->RemoveLight(this);

        Behaviour::OnDestroy();
    }

    //--------------------------------------------------------------
    Light *Light::Create() {
        return new Light();
    }

}