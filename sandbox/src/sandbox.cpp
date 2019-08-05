#include <hyperion/hyperion.hpp>

#include <stdio.h>

using namespace Hyperion;

class CSandboxApp : public CApplication {

    virtual void OnInit() override {
    
    }
    
    virtual void OnUpdate(float delta_time) override {

    }
    
    virtual void OnTick() override {
    
    }

};

Hyperion::CApplication *Hyperion::CreateApplication() {
    return new CSandboxApp();
}