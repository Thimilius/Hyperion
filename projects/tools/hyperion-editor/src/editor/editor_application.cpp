#include <hyperion/hyperion.hpp>
#include <hyperion/entry_point.hpp>

#include <hyperion/core/serialization/json_serializer.hpp>

namespace Hyperion::Editor {

    struct MyStruct {
        RTTR_ENABLE();
    public:
        String string;
        s32 number;

        RTTR_REGISTRATION_FRIEND;
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

RTTR_REGISTRATION
{
    using namespace rttr;
    using namespace Hyperion;
    using namespace Hyperion::Editor;

    registration::class_<MyStruct>(HYP_NAME_OF_TYPE(MyStruct))
        .constructor<>() 
        .property("string" , &MyStruct::string)
        .property("number", &MyStruct::number);
}

Hyperion::Application *Hyperion::CreateApplication() {
    ApplicationSettings settings = ApplicationSettings::FromJsonFile("app.json");
    return new Hyperion::Editor::EditorApplication(settings);
}