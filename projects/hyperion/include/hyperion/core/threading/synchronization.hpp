#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/threading/auto_reset_event.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion {
    class Engine;
    
    namespace Rendering {
        class RenderEngine;
    }
}

//-------------------- Definition Namespace --------------------
namespace Hyperion::Threading {

    class Synchronization final {
    private:
        Synchronization() = delete;
        ~Synchronization() = delete;
    private:
        inline static void NotifyRenderReady() { s_render_ready_event.Notify(); }
        inline static void WaitForRenderReady() { s_render_ready_event.Wait(); }

        inline static void NotifyUpdateReady() { s_update_ready_event.Notify(); }
        inline static void WaitForUpdateReady() { s_update_ready_event.Wait(); }

        inline static void NotifyRenderDone() { s_render_done_event.Notify(); }
        inline static void WaitForRenderDone() {s_render_done_event.Wait(); }

        inline static void NotifySwapDone() { s_swap_done_event.Notify(); }
        inline static void WaitForSwapDone() { s_swap_done_event.Wait(); }
    private:
        inline static AutoResetEvent s_render_ready_event = AutoResetEvent(false);
        inline static AutoResetEvent s_update_ready_event = AutoResetEvent(false);
        inline static AutoResetEvent s_render_done_event = AutoResetEvent(false);
        inline static AutoResetEvent s_swap_done_event = AutoResetEvent(false);
    private:
        friend class Hyperion::Engine;
        friend class Hyperion::Rendering::RenderEngine;
    };

}
