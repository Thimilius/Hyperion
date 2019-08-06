#include <hyperion/hyperion.hpp>

#include <stdio.h>

using namespace Hyperion;
using namespace Hyperion::Math;

class CSandboxApp : public CApplication {

    virtual void OnInit() override {
        HYP_TRACE("Hello Hyperion!");
    }
    
    virtual void OnUpdate(float delta_time) override {

    }
    
    virtual void OnTick() override {
        
    }

};

Hyperion::CApplication *Hyperion::CreateApplication() {
    return new CSandboxApp();
}