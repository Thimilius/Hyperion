#pragma once

#include "hyperion/common.hpp"

namespace Hyperion {

    template<typename T>
    class ICopyable {
    public:
        virtual ~ICopyable() = default;

        virtual T *Copy() const = 0;
    };

}