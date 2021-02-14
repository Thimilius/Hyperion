#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/common.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    class Random final {
    public:
        static float32 Get();
    private:
        Random() = delete;
        ~Random() = delete;
    };

}