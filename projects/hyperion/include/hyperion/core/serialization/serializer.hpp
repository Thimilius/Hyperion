#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/serialization/serializable.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    class Serializer final {
    public:
        static String Serialize(ISerializable *serializable);
        
        template<typename T>
        static T *DeserializeObject(const String &text) {
            T *object = new T();
            Deserialize(text, static_cast<ISerializable *>(object));
            return object;
        }
        
        template<typename T>
        static T DeserializeStruct(const String &text) {
            T object = { };
            Deserialize(text, static_cast<ISerializable *>(&object));
            return object;
        }
    private:
        static void Deserialize(const String &text, ISerializable *serializable);
    private:
        Serializer() = delete;
        ~Serializer() = delete;
    };

}