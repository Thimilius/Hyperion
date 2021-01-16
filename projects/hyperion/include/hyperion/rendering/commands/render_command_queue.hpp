#pragma once

#include "hyperion/common.hpp"
#include "hyperion/rendering/commands/render_commands.hpp"

namespace Hyperion::Rendering {

    class RenderCommandQueue {
    public:
        inline bool IsEmpty() const { return m_buffer.empty(); }
        inline void Clear() { m_buffer.clear(); }
        inline uint64 GetSize() const { return m_buffer.size(); }
        inline uint8 *GetData() { return m_buffer.data(); }

        inline void Allocate(RenderCommandType command_type) {
            AllocateInternal(command_type, sizeof(command_type));
        }

        template<typename T>
        inline T *Allocate(RenderCommandType command_type) {
            uint8 *data = AllocateInternal(command_type, sizeof(command_type) + sizeof(T));
            return reinterpret_cast<T *>(data);
        }
    private:
        inline uint8 *AllocateInternal(RenderCommandType command_type, uint64 size) {
            auto offset = m_buffer.size();
            m_buffer.resize(offset + size);
            uint8 *data = m_buffer.data() + offset;

            RenderCommandType *type_slot = reinterpret_cast<RenderCommandType *>(data);
            *type_slot = command_type;

            return data + sizeof(command_type);
        }
    private:
        Vector<uint8> m_buffer;
    };

}