//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/rendering/render_engine.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/core/app/window.hpp"
#include "hyperion/core/io/file_system.hpp"
#include "hyperion/core/profiling/profiling.hpp"
#include "hyperion/rendering/pipelines/forward/forward_render_pipeline.hpp"

//------------------------- Namespaces -------------------------
using namespace Hyperion::Graphics;

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

    //--------------------------------------------------------------
    void RenderEngine::PreInitialize(const RenderSettings &settings, Window *window) {
        s_render_settings = settings;

        s_graphics_context = window->CreateGraphicsContext(settings.graphics_backend);
        s_graphics_context->Initialize(GraphicsContextDescriptor());
        s_graphics_context->CreateDeviceAndSwapChain(&s_graphics_device, &s_graphics_device_context, &s_graphics_swap_chain);
    }

    //--------------------------------------------------------------
    void RenderEngine::Initialize() {
        switch (s_render_settings.pipeline) {
            case RenderPipeline::Forward: s_render_pipeline = new ForwardRenderPipeline(); break;
            case RenderPipeline::Custom: s_render_pipeline = s_render_settings.custom_pipeline; break;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE;
        }

        s_render_pipeline->Initialize();
    }

    //--------------------------------------------------------------
    void RenderEngine::Render() {
        {

        }
        s_graphics_context->SwapBuffers();
    }

    //--------------------------------------------------------------
    void RenderEngine::Shutdown() {
        delete s_render_pipeline;

        s_graphics_context->Shutdown();
        delete s_graphics_context;
    }

    //--------------------------------------------------------------
    void RenderEngine::InitializeGraphics() {
        Vector<uint8> vertex_shader_bytes = FileSystem::ReadAllBytes("data/shaders/spirv/vulkan_vertex.spirv");
        GraphicsShaderDescription vertex_shader_description = GraphicsShaderDescription();
        vertex_shader_description.type = GraphicsShaderType::Vertex;
        vertex_shader_description.byte_code_size = vertex_shader_bytes.size();
        vertex_shader_description.byte_code = vertex_shader_bytes.data();
        GraphicsShader *vertex_shader = s_graphics_device->CreateShader(vertex_shader_description);
        Vector<uint8> pixel_shader_bytes = FileSystem::ReadAllBytes("data/shaders/spirv/vulkan_pixel.spirv");
        GraphicsShaderDescription pixel_shader_description = GraphicsShaderDescription();
        pixel_shader_description.type = GraphicsShaderType::Pixel;
        pixel_shader_description.byte_code_size = pixel_shader_bytes.size();
        pixel_shader_description.byte_code = pixel_shader_bytes.data();
        GraphicsShader *pixel_shader = s_graphics_device->CreateShader(pixel_shader_description);
        
        GraphicsRenderPass *render_pass = s_graphics_device->CreateRenderPass(GraphicsRenderPassDescription());
        
        GraphicsPipelineStateDescription pipeline_state_description = GraphicsPipelineStateDescription();
        pipeline_state_description.vertex_shader = vertex_shader;
        pipeline_state_description.pixel_shader = pixel_shader;
        pipeline_state_description.render_pass = render_pass;
        GraphicsPipelineState *pipeline_state = s_graphics_device->CreatePipelineState(pipeline_state_description);
    }

}