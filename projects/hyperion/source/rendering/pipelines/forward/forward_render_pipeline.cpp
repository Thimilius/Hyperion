//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/rendering/pipelines/forward/forward_render_pipeline.hpp"

//---------------------- Library Includes ----------------------
#include <glad/glad.h>

//---------------------- Project Includes ----------------------
#include "hyperion/graphics/graphics_context.hpp"
#include "hyperion/rendering/frame/render_frame.hpp"
#include "hyperion/graphics/driver/opengl/opengl_shader_compiler.hpp"

//------------------------- Namespaces -------------------------
using namespace Hyperion::Graphics;

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

    GLuint g_shader_program;
    GLuint g_vertex_buffer;
    GLuint g_vertex_array;

    //--------------------------------------------------------------
    void ForwardRenderPipeline::Initialize(GraphicsContext *graphics_context) {
        graphics_context->CreateDeviceAndSwapChain(&m_device, &m_device_context, &m_swap_chain);

        const char *vertex_source = R"(
            #version 450
            
            layout(location = 0) in vec3 a_position;
            layout(location = 1) in vec3 a_texture0;
            layout(location = 2) in vec4 a_color;

            out V2F {
	            vec4 color;
            } o_v2f;

            uniform mat4 u_model;
            uniform mat4 u_view;
            uniform mat4 u_projection;

            void main() {
                o_v2f.color = a_color;
    
                gl_Position = u_projection * u_view * u_model * vec4(a_position, 1.0);
            }
        )";
        const char *fragment_source = R"(
            #version 450

            layout(location = 0) out vec4 o_color;

            in V2F {
	            vec4 color;
            } i_v2f;

            void main() {
                o_color = i_v2f.color;
            }	
        )";
        g_shader_program = OpenGLShaderCompiler::Compile(vertex_source, fragment_source).program;

        float32 positions[] = {
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  1.0f, 0.0f, 0.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  1.0f, 0.0f, 0.0f, 1.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  1.0f, 0.0f, 0.0f, 1.0f,

            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f, 1.0f, 0.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f, 1.0f, 0.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f, 1.0f, 0.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f, 1.0f, 0.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,  0.0f, 1.0f, 0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f, 1.0f, 0.0f, 1.0f,

            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f, 0.0f, 1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f, 0.0f, 1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f, 0.0f, 1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f, 0.0f, 1.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f, 0.0f, 1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f, 0.0f, 1.0f, 1.0f,

             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1.0f, 1.0f, 0.0f, 1.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  1.0f, 1.0f, 0.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  1.0f, 1.0f, 0.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  1.0f, 1.0f, 0.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  1.0f, 1.0f, 0.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1.0f, 1.0f, 0.0f, 1.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f, 1.0f, 1.0f, 1.0f,
             0.5f, -0.5f, -0.5f,  1.0f, 1.0f,  0.0f, 1.0f, 1.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f, 1.0f, 1.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f, 1.0f, 1.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f, 1.0f, 1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f, 1.0f, 1.0f, 1.0f,

            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1.0f, 0.0f, 1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1.0f, 0.0f, 1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,  1.0f, 0.0f, 1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
        };
        glCreateBuffers(1, &g_vertex_buffer);
        glNamedBufferData(g_vertex_buffer, sizeof(positions), positions, GL_STATIC_DRAW);

        glCreateVertexArrays(1, &g_vertex_array);

        GLsizei stride = sizeof(positions) / 36;
        glVertexArrayVertexBuffer(g_vertex_array, 0, g_vertex_buffer, 0, stride);

        glEnableVertexArrayAttrib(g_vertex_array, 0);
        glVertexArrayAttribFormat(g_vertex_array, 0, 3, GL_FLOAT, false, 0);
        glVertexArrayAttribBinding(g_vertex_array, 0, 0);

        glEnableVertexArrayAttrib(g_vertex_array, 1);
        glVertexArrayAttribFormat(g_vertex_array, 1, 2, GL_FLOAT, false, sizeof(Vector3));
        glVertexArrayAttribBinding(g_vertex_array, 1, 0);

        glEnableVertexArrayAttrib(g_vertex_array, 2);
        glVertexArrayAttribFormat(g_vertex_array, 2, 4, GL_FLOAT, false, sizeof(Vector3) + sizeof(Vector2));
        glVertexArrayAttribBinding(g_vertex_array, 2, 0);

        glEnable(GL_DEPTH_TEST);

        //glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CW);
    }

    //--------------------------------------------------------------
    void ForwardRenderPipeline::Render(RenderFrame *render_frame) {
        for (const RenderFrameCamera &render_frame_camera : render_frame->GetFrameCameras()) {
            RenderCamera(render_frame_camera, render_frame);
        }
    }

    //--------------------------------------------------------------
    void ForwardRenderPipeline::Shutdown() {

    }

    //--------------------------------------------------------------
    void ForwardRenderPipeline::RenderCamera(const RenderFrameCamera &render_frame_camera, RenderFrame *render_frame) {
        const RenderFrameCameraData &render_frame_camera_data = render_frame_camera.GetData();
        const CameraViewport &viewport = render_frame_camera_data.viewport;

        glProgramUniformMatrix4fv(g_shader_program, glGetUniformLocation(g_shader_program, "u_view"), 1, GL_FALSE, render_frame_camera_data.view_matrix.elements);
        glProgramUniformMatrix4fv(g_shader_program, glGetUniformLocation(g_shader_program, "u_projection"), 1, GL_FALSE, render_frame_camera_data.projection_matrix.elements);

        // TEMP: This is just so we see something on the screen.
        glViewport(viewport.x, viewport.y, viewport.width, viewport.height);
        Color background_color = render_frame_camera_data.background_color;
        glClearColor(background_color.r, background_color.g, background_color.b, background_color.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        glUseProgram(g_shader_program);
        glBindVertexArray(g_vertex_array);
        for (const RenderFrameObject &render_frame_object : render_frame->GetFrameObjects()) {
            const RenderFrameObjectData &render_frame_object_data = render_frame_object.GetData();
            glProgramUniformMatrix4fv(g_shader_program, glGetUniformLocation(g_shader_program, "u_model"), 1, GL_FALSE, render_frame_object_data.local_to_world.elements);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
    }

}