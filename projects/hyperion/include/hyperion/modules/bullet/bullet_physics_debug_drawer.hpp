#pragma once

#include <bullet/btBulletCollisionCommon.h>

namespace Hyperion::Physics {

    class BulletDebugDrawer : public btIDebugDraw {
    public:
        void drawContactPoint(const btVector3 &PointOnB, const btVector3 &normalOnB, btScalar distance, int lifeTime, const btVector3 &color) override { }
        void reportErrorWarning(const char *warningString) override { }
        void draw3dText(const btVector3 &location, const char *textString) override { }
        void setDebugMode(int debugMode) override { }
        int getDebugMode() const override { return DBG_DrawWireframe | DBG_DrawAabb; }

        void drawLine(const btVector3 &from, const btVector3 &to, const btVector3 &color) override;
    };

}