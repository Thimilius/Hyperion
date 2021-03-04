//--------------------- Definition Include ---------------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/core/serialization/reference_context.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    //--------------------------------------------------------------
    void ReferenceContext::Register(Guid guid, Object *object) {
        if (m_registered_objects.find(guid) == m_registered_objects.end()) {
            m_registered_objects[guid] = object;
        }
    }

    //--------------------------------------------------------------
    void ReferenceContext::Resolve(Guid guid, Object **object_pointer) {
        m_resolve_entries.emplace_back(guid, object_pointer);
    }

    //--------------------------------------------------------------
    void ReferenceContext::ResolveAllReferences() {
        for (const ReferenceResolveEntry &entry : m_resolve_entries) {
            auto it = m_registered_objects.find(entry.guid);
            if (it != m_registered_objects.end()) {
                *entry.object_pointer = it->second;
            } else {
                HYP_LOG_ERROR("Engine", "Failed to resolve reference to object with guid: '{}'", entry.guid.ToString());
            }
        }
    }

}