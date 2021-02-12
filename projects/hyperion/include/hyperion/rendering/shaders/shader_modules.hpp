#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/common.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion::Rendering {
    class RenderEngine;
}

//-------------------- Definition Namespace --------------------
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