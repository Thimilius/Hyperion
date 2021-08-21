#pragma once

//-------------------- Forward Declarations --------------------
namespace Hyperion::Rendering {
    class RenderEngine;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

    class RenderThread {
    private:
        RenderThread();
    private:
        friend class Hyperion::Rendering::RenderEngine;
    };

}