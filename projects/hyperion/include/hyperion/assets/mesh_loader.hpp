#pragma once

#include "hyperion/rendering/mesh.hpp"

namespace Hyperion {

    class IMeshLoader {
    public:
        virtual ~IMeshLoader() = default;

        virtual Rendering::Mesh *LoadMesh(const String &path) = 0;
    };

}