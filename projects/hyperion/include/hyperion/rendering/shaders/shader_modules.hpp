#pragma once

#include "hyperion/common.hpp"

namespace Hyperion::Rendering {
    class RenderEngine;
}

namespace Hyperion::Rendering {

    enum class ShaderModuleType {
        Unknown,
    };

    class ShaderModules final {
    public:
        static String GetModule(ShaderModuleType type);
    private:
        ShaderModules() = delete;
        ~ShaderModules() = delete;
    private:
        static void Initialize();
    private:
        inline static Map<ShaderModuleType, String> s_modules;
    private:
        friend class Hyperion::Rendering::RenderEngine;
    };

}