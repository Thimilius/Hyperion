//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/rendering/frame/render_frame.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {
    
    //--------------------------------------------------------------
    void RenderFrame::Clear() {
        m_assets_to_load.Clear();
        m_assets_to_unload.Clear();

        m_cameras.Clear();
        m_mesh_objects.Clear();
    }

    //--------------------------------------------------------------
    void RenderFrame::AddAssetToLoad(Asset *asset) {
        HYP_ASSERT(!m_assets_to_load.Contains(asset));
        m_assets_to_load.Add(asset);
    }

    //--------------------------------------------------------------
    void RenderFrame::AddAssetToUnload(AssetId asset_id) {
        HYP_ASSERT(!m_assets_to_unload.Contains(asset_id));
        m_assets_to_unload.Add(asset_id);
    }

    //--------------------------------------------------------------
    RenderFrameCamera &RenderFrame::AddCamera() {
        m_cameras.Resize(m_cameras.GetLength() + 1);
        return m_cameras.GetLast();
    }

    //--------------------------------------------------------------
    RenderFrameMeshObject &RenderFrame::AddMeshObject() {
        m_mesh_objects.Resize(m_mesh_objects.GetLength() + 1);
        return m_mesh_objects.GetLast();
    }

    //--------------------------------------------------------------
    RenderFrameSpriteObject &RenderFrame::AddSpriteObject() {
        m_sprite_objects.Resize(m_sprite_objects.GetLength() + 1);
        return m_sprite_objects.GetLast();
    }

}