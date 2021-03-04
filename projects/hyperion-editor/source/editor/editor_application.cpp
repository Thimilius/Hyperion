//--------------------- Definition Include ---------------------
#include "hyperion/editor/editor_application.hpp"

//---------------------- Library Includes ----------------------
#include <hyperion/entry_point.hpp>
#include <hyperion/core/app/application.hpp>
#include <hyperion/core/app/time.hpp>
#include <hyperion/core/io/file_system.hpp>
#include <hyperion/core/memory/memory.hpp>
#include <hyperion/core/serialization/serializer.hpp>
#include <hyperion/core/system/engine.hpp>
#include <hyperion/entity/world_manager.hpp>
#include <hyperion/scripting/scripting_engine.hpp>

//---------------------- Project Includes ----------------------
#include "hyperion/editor/rendering/editor_render_pipeline.hpp"
#include "hyperion/editor/world_view/editor_world_view.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Editor {

    //--------------------------------------------------------------
    void EditorApplication::OnInitialize() {
        EditorWorldView::Initialize();

        struct EmbeddedStruct : public ISerializable {
            int32 int_value;
            String string_value;

            void Serialize(ISerializationStream &stream) override {
                stream.WriteInt32("int_value", int_value);
                stream.WriteString("string_value", string_value);
            }

            void Deserialize(IDeserializationStream &stream) override {
                int_value = stream.ReadInt32("int_value");
                string_value = stream.ReadString("string_value");
            }
        };

        struct MyStruct : public ISerializable {
            int32 int_value;
            String string_value;
            MyStruct *pointer_value;
            EmbeddedStruct struct_value;

            void Serialize(ISerializationStream &stream) override {
                stream.WriteInt32("int_value", int_value);
                stream.WriteString("string_value", string_value);
                stream.WriteObject("pointer_value", pointer_value);
                stream.WriteStruct("struct_value", struct_value);
            }

            void Deserialize(IDeserializationStream &stream) override {
                int_value = stream.ReadInt32("int_value");
                string_value = stream.ReadString("string_value");
                pointer_value = stream.ReadObject<MyStruct>("pointer_value");
                struct_value = stream.ReadStruct<EmbeddedStruct>("struct_value");
            }
        };

        MyStruct inner_struct;
        inner_struct.int_value = 13;
        inner_struct.string_value = "Hello world!";
        inner_struct.pointer_value = nullptr;
        MyStruct my_struct;
        my_struct.pointer_value = &inner_struct;
        my_struct.string_value = "Hello there!";
        my_struct.struct_value.int_value = 10;
        my_struct.struct_value.string_value = "My String";
        String json = Serializer::Serialize(&my_struct);
        HYP_TRACE("\n{}", json);

        MyStruct deserialized_struct = Serializer::DeserializeStruct<MyStruct>(json);
    }

    //--------------------------------------------------------------
    void EditorApplication::OnUpdate(float32 delta_time) {
        if (Input::IsKeyHold(KeyCode::Control) && Input::IsKeyDown(KeyCode::W)) {
            Exit();
        }
        if (Input::IsKeyDown(KeyCode::F1)) {
            GetWindow()->SetWindowMode(GetWindow()->GetWindowMode() == WindowMode::Borderless ? WindowMode::Windowed : WindowMode::Borderless);
        }
        if (Input::IsKeyDown(KeyCode::F5)) {
            Engine::SetMode(Engine::GetMode() == EngineMode::Editor ? EngineMode::EditorRuntime : EngineMode::Editor);
        }

        EditorWorldView::Update(delta_time);
    }

}

//--------------------------------------------------------------
Hyperion::Application *Hyperion::CreateApplication() {
    ApplicationSettings settings = ApplicationSettings();
    settings.render.threading_mode = Rendering::RenderThreadingMode::MultiThreaded;
    settings.render.pipeline = Rendering::RenderPipeline::Custom;
    settings.render.custom_pipeline = new Editor::EditorRenderPipeline();
    return new Hyperion::Editor::EditorApplication(settings);
}