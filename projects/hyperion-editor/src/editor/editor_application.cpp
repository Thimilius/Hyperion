#include <hyperion/hyperion.hpp>
#include <hyperion/entry_point.hpp>

#include "hyperion/core/memory/memory.hpp"
#include "hyperion/core/serialization/json_serializer.hpp"

namespace Hyperion::Editor {

    struct MyStruct {
        HYP_REFLECT();

        String s;
        int32 i;
    };

    HYP_REFLECT_REGISTER_BEGIN
    {
        Registration<MyStruct>("MyStruct")
            .constructor()(DefaultConstructorPolicy)
            .property("s", &MyStruct::s)
            .property("i", &MyStruct::i);
    }
    HYP_REFLECT_REGISTER_END

    class EditorApplication : public Application {
    public:
        EditorApplication(const ApplicationSettings &settings) : Application(settings) { }
    protected:
        void OnInit() override {
            MyStruct *before_serialize = new MyStruct();
            before_serialize->s = "Hello there!";
            before_serialize->i = 17;
            
            JsonSerializer serializer;
            FileSystem::WriteAllText("file.json", serializer.Serialize(before_serialize));
            MyStruct *after_serialize = serializer.DeserializeRaw<MyStruct>(FileSystem::ReadAllText("file.json"));
            FileSystem::Delete("file.json");

            after_serialize->i;
        }

        void OnUpdate(float32 delta_time) override {
            if (Input::IsKeyHold(KeyCode::Control) && Input::IsKeyDown(KeyCode::W)) {
                Exit();
            }
            if (Input::IsKeyDown(KeyCode::F1)) {
                GetWindow()->SetWindowMode(GetWindow()->GetWindowMode() == WindowMode::Borderless ? WindowMode::Windowed : WindowMode::Borderless);
            }
        }

        void OnTick() override {
            float32 memory = static_cast<float32>(MemoryStats::GetGlobalMemory()) / 1000.0f / 1000.0f;
            String title = StringUtils::Format("Hyperion - FPS: {} ({:.2f}ms) - Memory: {:.2f}MB", Time::GetFPS(), Time::GetFrameTime(), memory);
            GetWindow()->SetTitle(title);
        }
    };

}

Hyperion::Application *Hyperion::CreateApplication() {
    ApplicationSettings settings = ApplicationSettings::FromJsonFile("app.json");
    settings.render.threading_mode = Rendering::RenderThreadingMode::MultiThreaded;
    return new Hyperion::Editor::EditorApplication(settings);
}