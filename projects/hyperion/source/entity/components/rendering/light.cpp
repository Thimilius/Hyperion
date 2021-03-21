//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/entity/components/rendering/light.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/entity/world.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    //--------------------------------------------------------------
    Rendering::LightData Light::GetData() const {
        Transform *transform = GetTransform();
        m_data.position = transform->GetPosition();
        m_data.direction = transform->GetForward();
        return m_data;
    }

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
    HYP_REFLECT_BEGIN(Light)
    HYP_REFLECT_BASE(Behaviour)
    HYP_REFLECT_CONSTRUCTOR([]() { return new Light(); })
    HYP_REFLECT_END()

}