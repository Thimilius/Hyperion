#include <hyperion/hyperion.hpp>

using namespace Hyperion;
using namespace Hyperion::Math;

class CSandboxApp : public CApplication {
public:
    CSandboxApp() : CApplication("", 1280, 720, EWindowMode::Windowed) { }
protected:
    void UpdateTitle() {
        GetWindow()->SetTitle(CString("Hyperion | FPS: %d (%.2f ms) | VSync: %s",
            CTime::GetFPS(),
            CTime::GetFrameTime(),
            CString(GetWindow()->GetVSyncMode() != EVSyncMode::DontSync).ToCString()));
    }

    void OnInit() override {
        GetWindow()->SetVSyncMode(EVSyncMode::EveryVBlank);
        GetWindow()->SetIcon("logo/logo.ico");

        UpdateTitle();
    }
    
    void OnEvent(CEvent &event) override {

    }

    void OnUpdate(float delta_time) override {
        if (CInput::GetKey(EKeyCode::Control) && CInput::GetKeyDown(EKeyCode::W)) {
            Exit();
        }

        if (CInput::GetKeyDown(EKeyCode::F1)) {
            GetWindow()->SetWindowMode(GetWindow()->GetWindowMode() == EWindowMode::Windowed ? EWindowMode::Borderless : EWindowMode::Windowed);
        }
        if (CInput::GetKeyDown(EKeyCode::F2)) {
            GetWindow()->SetVSyncMode(GetWindow()->GetVSyncMode() == EVSyncMode::DontSync ? EVSyncMode::EveryVBlank : EVSyncMode::DontSync);
            UpdateTitle();
        }
    }
    
    void OnTick() override {
        UpdateTitle();
    }
};

Hyperion::CApplication *Hyperion::CreateApplication() {
    return new CSandboxApp();
}