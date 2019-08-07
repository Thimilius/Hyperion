#include <hyperion/hyperion.hpp>

#include <stdio.h>

using namespace Hyperion;
using namespace Hyperion::Math;

class CSandboxApp : public CApplication {
public:
    CSandboxApp() : CApplication(CString("Hyperion | FPS: 0 (0 ms)"), 1280, 720, EWindowMode::Windowed) {

    }
protected:
    void OnInit() override {
        GetWindow()->SetVSyncMode(EVSyncMode::EveryVBlank);
    }
    
    void OnEvent(CEvent &event) override {

    }

    void OnFixedUpdate(float delta_time) override {

    }

    void OnUpdate(float delta_time) override {
        if (CInput::GetKey(EKeyCode::Control) && CInput::GetKeyDown(EKeyCode::W)) {
            Exit();
        }
    }
    
    void OnTick() override {
        GetWindow()->SetTitle(CString("Hyperion | FPS: %d (%.2f ms)", CTime::GetFPS(), CTime::GetFrameTime()));
    }
};

Hyperion::CApplication *Hyperion::CreateApplication() {
    return new CSandboxApp();
}