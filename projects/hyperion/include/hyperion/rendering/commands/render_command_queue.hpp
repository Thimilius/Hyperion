#pragma once

#include "hyperion/common.hpp"
#include "hyperion/rendering/commands/render_commands.hpp"

namespace Hyperion::Rendering {

    class RenderCommandQueue {
    public:
        inline bool IsEmpty() const { return m_buffer.empty(); }
        inline void Clear() { m_buffer.clear(); }
        inline u64 GetSize() const { return m_buffer.size(); }
        inline u8 *GetData() { return m_buffer.data(); }

        inline void Allocate(RenderCommandType command_type) {
            AllocateInternal(command_type, sizeof(command_type));
        }

        template<typename T>
        inline T *Allocate(RenderCommandType command_type) {
            u8 *data = AllocateInternal(command_type, sizeof(command_type) + sizeof(T));
            return reinterpret_cast<T *>(data);
        }
    private:
        inline u8 *AllocateInternal(RenderCommandType command_type, u64 size) {
            auto offset = m_buffer.size();
            m_buffer.resize(offset + size);
            u8 *data = m_buffer.data() + offset;

            RenderCommandType *type_slot = reinterpret_cast<RenderCommandType *>(data);
            *type_slot = command_type;

            return data + sizeof(command_type);
        }
    private:
        Vector<u8> m_buffer;
    };

}