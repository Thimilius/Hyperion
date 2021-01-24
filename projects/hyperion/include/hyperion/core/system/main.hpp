#pragma once

#include "hyperion/common.hpp"

namespace Hyperion {

    class Main final {
    public:
        static uint32 Run();
    private:
        Main() = delete;
        ~Main() = delete;
    };

}