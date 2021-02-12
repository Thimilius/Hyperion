//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/modules/bullet/bullet_physics_debug_drawer.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/rendering/immediate_renderer.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Physics {

    //--------------------------------------------------------------
    void BulletDebugDrawer::drawLine(const btVector3 &from, const btVector3 &to, const btVector3 &color) {
        Rendering::ImmediateRenderer::DrawLine(Vec3(from.x(), from.y(), from.z()), Vec3(to.x(), to.y(), to.z()), Color(color.x(), color.y(), color.z(), 1.0f));
    }
}