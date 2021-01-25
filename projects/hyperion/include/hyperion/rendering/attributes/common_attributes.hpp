#pragma once

#include "hyperion/common.hpp"

namespace Hyperion::Rendering {

    template<typename T>
    struct ArrayDescriptor {
        uint64 size; // This size is always in bytes!
        const T *data;

        ArrayDescriptor() = default;

        ArrayDescriptor(const Vector<T> &vector) {
            size = vector.size() * sizeof(vector[0]);
            data = vector.data();
        }

        ArrayDescriptor(const String &string) {
            size = string.size() + 1; // We have to take into account the null termination character
            data = string.data();
        }
    };

}