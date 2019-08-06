#include <hyperion/hyperion.hpp>

#include <stdio.h>

using namespace Hyperion;
using namespace Hyperion::Math;

class CSandboxApp : public CApplication {
public:
    CSandboxApp() : CApplication(CString("Hyperion | FPS: 0 (0 ms)"), 1280, 720, EWindowMode::Windowed) {

    }
protected:
    virtual void OnInit() override {
        GetWindow()->SetVSyncMode(EVSyncMode::EveryVBlank);
    }
    
    virtual void OnEvent(CEvent &event) override {
        if (event.GetType() == EEventType::WindowFocus) {
            CWindowFocusEvent *focus_event = (CWindowFocusEvent*)(&event);
            HYP_TRACE("%s: %d", event.GetName().ToCString(), focus_event->GetFocus());
        }
    }

    virtual void OnFixedUpdate(float delta_time) override {

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