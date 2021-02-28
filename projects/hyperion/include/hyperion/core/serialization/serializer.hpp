#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/serialization/serializable.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    class Serializer final {
    public:
        static void Serialize(ISerializable *serializable);
    private:
        Serializer() = delete;
        ~Serializer() = delete;
    };

}