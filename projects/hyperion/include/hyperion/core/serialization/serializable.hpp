#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/serialization/serialization_stream.hpp"
#include "hyperion/core/serialization/reference_context.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    class ISerializable {
    public:
        virtual ~ISerializable() = default;
    public:
        virtual void Serialize(ISerializationStream &stream) = 0;
        virtual void Deserialize(IDeserializationStream &stream, ReferenceContext &context) = 0;
    };

}