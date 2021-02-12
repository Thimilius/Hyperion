//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/rendering/pipelines/render_pipeline.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

    //--------------------------------------------------------------
    RenderPipelineContext::RenderPipelineContext(const CameraData &camera_data, const Vector<MeshRenderer *> &mesh_renderers)
        : m_camera_data(camera_data), m_mesh_renderers(mesh_renderers) { }

}