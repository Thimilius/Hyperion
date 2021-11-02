//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/threading/auto_reset_event.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion::Rendering {
    class RenderEngine;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

    class RenderThreadSynchronization final {
    private:
        RenderThreadSynchronization() = delete;
        ~RenderThreadSynchronization() = delete;
    private:
        inline static void NotifyRenderReady() { s_render_ready_event.Notify(); }
        inline static void WaitForRenderReady() { s_render_ready_event.Wait(); }

        inline static void NotifyMainReady() { s_main_ready_event.Notify(); }
        inline static void WaitForMainReady() { s_main_ready_event.Wait(); }

        inline static void NotifyRenderDone() { s_render_done_event.Notify(); }
        inline static void WaitForRenderDone() {s_render_done_event.Wait(); }

        inline static void NotifySwapDone() { s_swap_done_event.Notify(); }
        inline static void WaitForSwapDone() { s_swap_done_event.Wait(); }
    private:
        inline static Threading::AutoResetEvent s_render_ready_event = Threading::AutoResetEvent(false);
        inline static Threading::AutoResetEvent s_main_ready_event = Threading::AutoResetEvent(false);
        inline static Threading::AutoResetEvent s_render_done_event = Threading::AutoResetEvent(false);
        inline static Threading::AutoResetEvent s_swap_done_event = Threading::AutoResetEvent(false);
    private:
        friend class Hyperion::Rendering::RenderEngine;
    };

}
