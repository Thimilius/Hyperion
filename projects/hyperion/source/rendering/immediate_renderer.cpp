//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/rendering/immediate_renderer.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/core/app/display.hpp"
#include "hyperion/core/io/file_system.hpp"
#include "hyperion/rendering/render_engine.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {
    
    //--------------------------------------------------------------
    void ImmediateRenderer::Begin(MeshTopology topology) {
        s_current_topology = topology;
        s_current_index = 0;

        s_immediate_mesh_data.positions.clear();
        s_immediate_mesh_data.colors.clear();
        s_immediate_mesh_data.indices.clear(),

        Object::Destroy(s_immediate_mesh);
    }

    //--------------------------------------------------------------
    void ImmediateRenderer::DrawLine(Vec3 start, Vec3 end, Color color) {
        AddVertex(start, color);
        AddVertex(end, color);
    }

    //--------------------------------------------------------------
    void ImmediateRenderer::End() {
        if (s_current_index > 0) {
            Vector<SubMesh> sub_meshes = { { s_current_topology, s_current_index, 0, 0 } };
            s_immediate_mesh = Mesh::Create(s_immediate_mesh_data, sub_meshes);

            s_render_driver->DrawMesh(s_immediate_mesh->GetResourceId(), Mat4::Identity(), s_immediate_material->GetResourceId(), 0);
        }
    }

    //--------------------------------------------------------------
    void ImmediateRenderer::Initialize(IRenderDriver *render_driver) {
        s_render_driver = render_driver;

        String &immediate_source = FileSystem::ReadAllText("data/shaders/immediate.shader");
        s_immediate_material = Material::Create(Shader::Create(immediate_source));
    }

    //--------------------------------------------------------------
    void ImmediateRenderer::AddVertex(Vec3 position, Color color) {
        s_immediate_mesh_data.positions.push_back(position);
        s_immediate_mesh_data.colors.push_back(color);
        s_immediate_mesh_data.indices.push_back(s_current_index++);
    }

}