//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/rendering/frame/render_frame.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {
    
    //--------------------------------------------------------------
    void RenderFrame::Reset() {
        m_frame_assets_to_unload.Clear();

        m_frame_cameras.Clear();
        m_frame_mesh_objects.Clear();
    }

    //--------------------------------------------------------------
    void RenderFrame::AddFrameAssetToUnload(AssetId asset_id) {
        HYP_ASSERT(!m_frame_assets_to_unload.Contains(asset_id));
        m_frame_assets_to_unload.Add(asset_id);
    }

    //--------------------------------------------------------------
    RenderFrameCamera &RenderFrame::AddFrameCamera() {
        m_frame_cameras.Resize(m_frame_cameras.GetLength() + 1);
        return m_frame_cameras.GetLast();
    }

    //--------------------------------------------------------------
    RenderFrameMeshObject &RenderFrame::AddFrameMeshObject() {
        m_frame_mesh_objects.Resize(m_frame_mesh_objects.GetLength() + 1);
        return m_frame_mesh_objects.GetLast();
    }

    //--------------------------------------------------------------
    RenderFrameSpriteObject &RenderFrame::AddFrameSpriteObject() {
        m_frame_sprite_objects.Resize(m_frame_sprite_objects.GetLength() + 1);
        return m_frame_sprite_objects.GetLast();
    }

}