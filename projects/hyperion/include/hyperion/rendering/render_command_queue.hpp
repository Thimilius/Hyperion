#pragma once

#include "hyperion/rendering/render_command.hpp"

namespace Hyperion::Rendering {

    class RenderCommandQueue {
    public:
        inline bool IsEmpty() const { return m_queue.empty(); }

        inline void Push(const RenderCommand &command) {
            m_queue.push(command);
        }

        inline RenderCommand Pop() {
            RenderCommand command = m_queue.front();
            m_queue.pop();
            return command;
        }
    private:
        Queue<RenderCommand> m_queue;
    };

}