#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/entity/components/ui/widget.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    class Graphic : public Widget {
        HYP_REFLECT(Widget);
    protected:
        void OnCreate() override;
        void OnRebuildMesh(MeshBuilder &mesh_builder) override;
    private:
        Graphic() : Widget("Graphic") {}
    private:
        static Graphic *Create();
    };

}