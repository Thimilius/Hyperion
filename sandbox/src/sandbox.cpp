#include <hyperion/hyperion.hpp>

using namespace Hyperion;
using namespace Hyperion::Events;
using namespace Hyperion::Math;
using namespace Hyperion::Rendering;

class CSandboxApp : public CApplication {
public:
    CSandboxApp() : CApplication("Hyperion", 1280, 720, EWindowMode::Windowed) { }
protected:
    void UpdateTitle() {
        CString title = CString::Format("Hyperion | FPS: {} ({:.2f} ms) | Vsync: {}", CTime::GetFPS(), CTime::GetFrameTime(), GetWindow()->GetVSyncMode() != EVSyncMode::DontSync);
        GetWindow()->SetTitle(title);
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
            GetWindow()->SetWindowMode(EWindowMode::Windowed);
        }
        if (CInput::GetKeyDown(EKeyCode::F2)) {
            GetWindow()->SetWindowMode(EWindowMode::Borderless);
        }
        if (CInput::GetKeyDown(EKeyCode::F3)) {
            GetWindow()->SetWindowMode(EWindowMode::Fullscreen);
        }
        if (CInput::GetKeyDown(EKeyCode::F4)) {
            GetWindow()->SetVSyncMode(GetWindow()->GetVSyncMode() == EVSyncMode::DontSync ? EVSyncMode::EveryVBlank : EVSyncMode::DontSync);
            UpdateTitle();
        }
    }

    void OnRender() override {
        CRenderCommand::SetClearColor(0, 1, 1, 1);
        CRenderCommand::Clear();
    }
    
    void OnTick() override {
        UpdateTitle();
    }
};

Hyperion::CApplication *Hyperion::CreateApplication() {
    return new CSandboxApp();
}