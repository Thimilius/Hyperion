#pragma once

#include "hyperion/common.hpp"

namespace Hyperion {

    template<typename T>
    class Copyable {
    public:
        virtual ~Copyable() = default;

        virtual Ref<T> Copy() const = 0;
    };

}