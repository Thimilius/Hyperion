#include <hyperion/hyperion.hpp>
#include <hyperion/entry_point.hpp>

#include <hyperion/core/threading/thread.hpp>
#include <hyperion/core/serialization/json_serializer.hpp>

namespace Hyperion::Editor {

    struct MyStruct {
        HYP_REFLECT();

        String string;
        s32 number;
    };

    class EditorApplication : public Application {
    public:
        EditorApplication(const ApplicationSettings &settings) : Application(settings) { }
    protected:
        void OnInit() override {
            MyStruct before;
            before.string = "Hello there";
            before.number = 123;

            JsonSerializer serializer;
            serializer.Serialize("file.json", before);
            HYP_TRACE("Before - string: {}; number: {}", before.string, before.number);
            MyStruct after = serializer.Deserialize<MyStruct>("file.json");
            HYP_TRACE("After - string: {}; number: {}", after.string, after.number);
            FileSystem::Delete("file.json");
        }

        void OnUpdate(f32 delta_time) override {
            
        }

        void OnTick() override {

        }

        void OnEvent(Event &event) override {
            
        }

        void OnShutdown() override {

        }
    };

}

HYP_REFLECT_REGISTER_BEGIN
{
    registration::class_<Editor::MyStruct>("MyStruct")
        .constructor<>()
        .property("string", &Editor::MyStruct::string)
        .property("number", &Editor::MyStruct::number);
}
HYP_REFLECT_REGISTER_END

Hyperion::Application *Hyperion::CreateApplication() {
    ApplicationSettings settings = ApplicationSettings::FromJsonFile("app.json");
    return new Hyperion::Editor::EditorApplication(settings);
}