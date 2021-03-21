//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/core/object/type.hpp"

#include "hyperion/core/object/object.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    //--------------------------------------------------------------
    Type::Type(TypeInitializerFunction initializer_function) {
        HYP_ASSERT(initializer_function);
        HYP_ASSERT(m_data == nullptr);

        m_data = initializer_function();
        s_types.push_back(this);
    }

    //--------------------------------------------------------------
    bool Type::IsDerivedFrom(Type *base) {
        if (base == nullptr) {
            return false;
        }

        if (m_data == base->m_data) {
            return true;
        }

        if (m_data->base != nullptr) {
            return m_data->base->IsDerivedFrom(base);
        } else {
            return false;
        }
    }

    //--------------------------------------------------------------
    Type *Type::GetByName(const String &name) {
        for (Type *type : s_types) {
            if (type->GetName() == name) {
                return type;
            }
        }
        return nullptr;
    }

}