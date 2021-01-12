#pragma once

#include "hyperion/common.hpp"
#include "hyperion/rendering/render_command.hpp"

namespace Hyperion::Rendering {

    using RenderCommandVisitorFunction = std::function<u64(RenderCommandType, const void *)>;

    class RenderCommandQueue {
    public:
        inline bool IsEmpty() const { return m_buffer.empty(); }
        inline void Clear() { m_buffer.clear(); }

        template<typename T>
        inline T *Allocate(RenderCommandType command_type) {
            auto offset = m_buffer.size();
            m_buffer.resize(offset + sizeof(RenderCommandType) + sizeof(T));
            u8 *data = m_buffer.data() + offset;

            RenderCommandType *type_slot = reinterpret_cast<RenderCommandType *>(data);
            *type_slot = command_type;

            return reinterpret_cast<T *>(data + sizeof(RenderCommandType));
        }
    private:
        Vector<u8> m_buffer;

        friend class Hyperion::Rendering::RenderEngine;
    };

}