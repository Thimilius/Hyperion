#pragma once

#include "hyperion/core/threading/auto_reset_event.hpp"

namespace Hyperion {
    class Engine;

    namespace Rendering {
        class RenderEngine;
    }
}

namespace Hyperion {

    class Synchronization final {
    private:
        inline static void NotifyRenderReady() { s_render_ready_event.Notify(); }
        inline static void WaitForRenderReady() { s_render_ready_event.Wait(); }

        inline static void NotifyUpdateReady() { s_update_ready_event.Notify(); }
        inline static void WaitForUpdateReady() { s_update_ready_event.Wait(); }

        inline static void NotifyRenderDone() { s_render_done_event.Notify(); }
        inline static void WaitForRenderDone() { s_render_done_event.Wait(); }

        inline static void NotifySwapDone() { s_swap_done_event.Notify(); }
        inline static void WaitForSwapDone() { s_swap_done_event.Wait(); }
    private:
        inline static Threading::AutoResetEvent s_render_ready_event = Threading::AutoResetEvent(false);
        inline static Threading::AutoResetEvent s_update_ready_event = Threading::AutoResetEvent(false);
        inline static Threading::AutoResetEvent s_render_done_event = Threading::AutoResetEvent(false);
        inline static Threading::AutoResetEvent s_swap_done_event = Threading::AutoResetEvent(false);
    private:
        friend class Hyperion::Engine;
        friend class Hyperion::Rendering::RenderEngine;
    };

}