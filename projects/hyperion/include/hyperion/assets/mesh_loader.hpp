#pragma once

#include "hyperion/rendering/mesh.hpp"

namespace Hyperion {

    class MeshLoader {
    public:
        virtual ~MeshLoader() = default;

        virtual Ref<Rendering::Mesh> LoadMesh(const String &path) = 0;
    };

}