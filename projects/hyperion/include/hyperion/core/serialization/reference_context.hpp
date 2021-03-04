#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/object/guid.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion {
    class Object;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    class ReferenceContext {
    private:
        struct ReferenceResolveEntry {
            Guid guid;
            Object **object_pointer;

            ReferenceResolveEntry(Guid guid, Object **object_pointer) : guid(guid), object_pointer(object_pointer) { }
        };
    public:
        void Register(Guid guid, Object *object);
        template<typename T>
        void Resolve(Guid guid, T **object_pointer) {
            Resolve(guid, reinterpret_cast<Object **>(object_pointer));
        }
        void Resolve(Guid guid, Object **object_pointer);
        void ResolveAllReferences();
    private:
        Vector<ReferenceResolveEntry> m_resolve_entries;
        Map<Guid, Object *> m_registered_objects;
    };

}