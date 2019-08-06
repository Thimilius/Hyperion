#include <hyperion/hyperion.hpp>

#include <stdio.h>

using namespace Hyperion;
using namespace Hyperion::Math;

class CSandboxApp : public CApplication {

    virtual void OnInit() override {
        GetWindow()->SetTitle(CString("Hyperion | FPS: %d (%.2f ms)", CTime::GetFPS(), CTime::GetFrameTime()));
        GetWindow()->SetVSyncMode(EVSyncMode::EveryVBlank);
    }
    
    virtual void OnUpdate(float delta_time) override {

    }
    
    virtual void OnTick() override {
        GetWindow()->SetTitle(CString("Hyperion | FPS: %d (%.2f ms)", CTime::GetFPS(), CTime::GetFrameTime()));
    }

};

Hyperion::CApplication *Hyperion::CreateApplication() {
    return new CSandboxApp();
}