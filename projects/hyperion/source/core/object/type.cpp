//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/core/object/type.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    //--------------------------------------------------------------
    MyType::MyType(TypeInitializerFunction initializer_function) {
        HYP_ASSERT(initializer_function);
        HYP_ASSERT(m_data == nullptr);

        m_data = initializer_function();
    }

    //--------------------------------------------------------------
    bool MyType::IsDerivedFrom(MyType base) {
        if (!IsValid() || !base.IsValid()) {
            return false;
        }

        if (m_data == base.m_data) {
            return true;
        }

        if (m_data->base != nullptr) {
            return m_data->base->IsDerivedFrom(base);
        } else {
            return false;
        }
    }

}