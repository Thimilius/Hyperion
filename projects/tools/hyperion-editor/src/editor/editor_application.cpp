﻿#include <hyperion/hyperion.hpp>
#include <hyperion/entry_point.hpp>

#include "hyperion/editor/editor_engine.hpp"

namespace Hyperion::Editor {

    class EditorApplication : public Application {
    public:
        EditorApplication(const ApplicationSettings &settings) : Application(settings) { }
    protected:
        void OnInit() override {
            EditorEngine::Init();
        }

        void OnUpdate(f32 delta_time) override {
            EditorEngine::Update(delta_time);
        }

        void OnRender() override {
            EditorEngine::Render();
        }

        void OnTick() override {
            EditorEngine::Tick();
        }
    };

}

void Hyperion::RegisterApplicationTypes() {

}

Hyperion::Application *Hyperion::CreateApplication() {
    return new Hyperion::Editor::EditorApplication(ApplicationSettings::FromJsonFile("app.json"));
}