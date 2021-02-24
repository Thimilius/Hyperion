#pragma once

//---------------------- Library Includes ----------------------
#include <mono/jit/jit.h>

//---------------------- Project Includes ----------------------
#include "hyperion/core/object/object.hpp"
#include "hyperion/entity/components/script.hpp"
#include "hyperion/modules/mono/mono_scripting_instance.hpp"
#include "hyperion/modules/mono/mono_scripting_type.hpp"
#include "hyperion/scripting/scripting_driver.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Scripting {

    class MonoScriptingDriver : public IScriptingDriver {
    public:
        uint64 GetMemoryUsage() const override;

        void Initialize(const ScriptingSettings &settings) override;
        void EngineModeChange(EngineMode engine_mode) override;
        void Update() override;
        void Shutdown() override;
    public:
        inline static MonoDomain *GetDomain() { return s_root_domain; }
        
        inline static MonoClass *GetComponentClass() { return s_core_component_class; }
        inline static MonoClass *GetScriptClass() { return s_core_script_class; }

        static void InvokeMethod(MonoMethod *method, void *object, void **parameters);

        template<typename T>
        static T *GetNativeObjectAs(MonoObject *managed_object) {
            return reinterpret_cast<T *>(GetNativeObject(managed_object));
        }
        static void *GetNativeObject(MonoObject *managed_object);
        static MonoObject *GetOrCreateManagedObject(Object *native_object, Type native_type);
        static MonoObject *GetOrCreateManagedObjectRaw(void *native, Type native_type);
        static MonoObject *CreateManagedObjectFromManagedType(Object *native_object, MonoClass *managed_class, bool is_script_component);
        static MonoObject *CreateManagedObjectFromNativeType(Object *native_object, Type native_type);

        static bool IsRegisterdObject(MonoObject *managed_object);
        static void RegisterManagedObject(MonoObject *managed_object, Object *native_object, bool is_script_component);
        static void RegisterObject(MonoObject *managed_object, void *native);
        static void UnregisterObject(MonoObject *managed_object, bool is_script_component);
        
        static bool IsNativeClass(MonoClass *native_class);
        static Type GetNativeClass(MonoClass *native_class);
        static void RegisterClass(Type native_class, const char *managed_namespace, const char *managed_name);

        static MonoScriptingType *GetOrCreateScriptingType(MonoClass *managed_class);
    private:
        static void InitDebugger(const ScriptingSettings &settings);
        static void InitDomain();
        static void InitBindings();

        static void ReloadScriptingDomain();
        static void UnloadScriptingDomain();

        static void PrintUnhandledException(MonoObject *exception);
    private:
        inline static MonoDomain *s_root_domain;
        inline static MonoDomain *s_scripting_domain;

        inline static MonoAssembly *s_core_assembly;
        inline static MonoImage *s_core_assembly_image;
        inline static MonoClass *s_core_component_class;
        inline static MonoClass *s_core_script_class;

        inline static MonoAssembly *s_editor_assembly;
        inline static MonoImage *s_editor_assembly_image;
        inline static MonoMethod *s_editor_update_method;

        inline static Map<MonoObject *, void *> s_managed_to_native_objects;
        inline static Map<void *, MonoObject * > s_native_to_managed_objects;

        inline static Map<Type, MonoClass *> s_native_to_managed_classes;
        // NOTE: The variant just stores a Type.
        // We can not use Type directly because it does not have a public default constructor.
        inline static Map<MonoClass *, Variant> s_managed_to_native_classes; 

        inline static Vector<Script *> s_scripts_which_recieve_messages;
        inline static Map<MonoClass *, MonoScriptingType *> s_scripting_types;
    };

}