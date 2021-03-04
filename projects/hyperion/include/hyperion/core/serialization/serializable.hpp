#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/serialization/serialization_stream.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    class ISerializable {
    public:
        virtual ~ISerializable() = default;
    public:
        virtual void Serialize(ISerializationStream &stream) = 0;
        virtual void Deserialize(IDeserializationStream &stream) = 0;
    };

}