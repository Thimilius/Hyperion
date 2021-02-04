#pragma once

#include "hyperion/common.hpp"
#include "hyperion/rendering/threading/render_thread_commands.hpp"

namespace Hyperion::Rendering {
    class RenderEngine;
}

namespace Hyperion::Rendering {

    template<typename CommandType>
    class RenderThreadCommandQueue {
    private:
        inline static constexpr uint64 MINIMIUM_CAPACITY_BEFORE_SHRINK = 1024 * 1024 * 10;
    public:
        inline void Allocate(CommandType command_type) {
            AllocateInternal(command_type, sizeof(command_type));
        }

        template<typename T>
        inline T *Allocate(CommandType command_type, uint64 extra_size = 0) {
            uint8 *data = AllocateInternal(command_type, sizeof(command_type) + sizeof(T) + extra_size);
            return reinterpret_cast<T *>(data);
        }
    private:
        inline uint64 GetSize() const { return m_buffer.size(); }
        inline uint8 *GetData() { return m_buffer.data(); }

        inline void Clear() {
            m_buffer.clear();

            // The queue should always have a certain size reserved even when shrinking.
            if (m_buffer.capacity() > MINIMIUM_CAPACITY_BEFORE_SHRINK) {
                m_buffer.shrink_to_fit();
                m_buffer.reserve(MINIMIUM_CAPACITY_BEFORE_SHRINK);
            }
        }

        inline void Reserve() {
            m_buffer.reserve(MINIMIUM_CAPACITY_BEFORE_SHRINK);
        }

        inline uint8 *AllocateInternal(CommandType command_type, uint64 size) {
            auto offset = m_buffer.size();
            m_buffer.resize(offset + size);
            uint8 *data = m_buffer.data() + offset;

            CommandType *type_slot = reinterpret_cast<CommandType *>(data);
            *type_slot = command_type;

            return data + sizeof(command_type);
        }
    private:
        Vector<uint8> m_buffer;
    private:
        friend class Rendering::RenderEngine;
    };

    // This could probably be implemented inside the render thread command queue itself!?
    // NOTE: We have to remember that the array descriptors have to be handled very carefully.
    // Because they contain a pointer to the data and the whole command queue might be reallocated,
    // we have to update that pointer in the Render Thread before usage with the Read operation and
    // can not do it beforehand when copying with the Write operation.
    template<typename Command>
    class RenderThreadCommandQueueHelper {
    public:
        RenderThreadCommandQueueHelper(Command *data) {
            m_data_pointer = reinterpret_cast<uint8 *>(data + 1);
        }

        template<typename T>
        inline void Write(const ArrayDescriptor<T> &source) {
            uint64 size = source.size;
            std::memcpy(m_data_pointer, source.data, size);
            m_data_pointer += size;
        }

        template<typename T>
        inline void Read(ArrayDescriptor<T> &destination) {
            uint64 size = destination.size;
            destination.data = reinterpret_cast<T *>(m_data_pointer);
            m_data_pointer += size;
        }
    private:
        uint8 *m_data_pointer;
    };

}