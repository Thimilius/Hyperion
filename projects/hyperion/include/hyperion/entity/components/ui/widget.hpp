#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/assets/mesh.hpp"
#include "hyperion/entity/components/behaviour.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion {
    class UiEngine;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    class Widget : public Behaviour {
        HYP_REFLECT(Behaviour);
    public:
        bool IsDirty() const { return m_is_dirty; }
    protected:
        Widget() = default;
        Widget(const String & name) : Behaviour(name) { }
    protected:
        void SetDirty() { m_is_dirty = true; }
        void ResetDirty() { m_is_dirty = false; }

        void OnCreate() override;
        void OnDestroy() override;

        virtual void OnRebuildMesh(MeshBuilder &mesh_builder) { }
    protected:
        bool m_is_dirty = false;
    private:
        friend class Hyperion::UiEngine;
    }; 

}