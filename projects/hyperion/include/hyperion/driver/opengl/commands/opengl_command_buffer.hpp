#pragma once

#include "hyperion/driver/opengl/commands/opengl_commands.hpp"
#include "hyperion/rendering/command_buffer.hpp"

namespace Hyperion::Rendering {

    class OpenGLCommandBuffer : public CommandBuffer {
    public:
        inline uint64 GetSize() const { return m_buffer.size(); }
        inline uint8 *GetData() { return m_buffer.data(); }

        void ClearCommands() override;

        void Clear(ClearFlags clear_flags, Color color) override;
        void SetViewport(const Viewport &viewport) override;
        void SetRasterizerState(const RasterizerState &rasterizer_state) override;

        void SetRenderTexture(RenderTexture *render_texture) override;
        void Blit(RenderTexture *destination, RenderTexture *source) override;

        void DrawMesh(Mesh *mesh, const Mat4 &transformation_matrix, Material *material, uint32 sub_mesh_index) override;
    private:
        template<typename T>
        inline T *Allocate(OpenGLCommandType command_type, uint64 extra_size = 0) {
            uint64 size = sizeof(command_type) + sizeof(T) + extra_size;

            auto offset = m_buffer.size();
            m_buffer.resize(offset + size);
            uint8 *data = m_buffer.data() + offset;

            OpenGLCommandType *type_slot = reinterpret_cast<OpenGLCommandType *>(data);
            *type_slot = command_type;

            return reinterpret_cast<T *>(data + sizeof(command_type));
        }
    private:
        Vector<uint8> m_buffer;
    };

}