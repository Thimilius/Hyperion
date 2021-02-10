#pragma once

#include <mono/jit/jit.h>

#include "hyperion/core/object/object.hpp"
#include "hyperion/scripting/scripting_driver.hpp"
#include "hyperion/modules/mono/mono_scripting_type.hpp"

namespace Hyperion::Scripting {

    class MonoScriptingDriver : public IScriptingDriver {
    public:
        uint64 GetMemoryUsage() const override;

        void Initialize(const ScriptingSettings &settings) override;
        void Update() override;
        void Shutdown() override;
    public:
        inline static MonoDomain *GetDomain() { return s_core_domain; }

        inline static MonoClass *GetComponentClass() { return s_component_class; }
        inline static MonoClass *GetScriptClass() { return s_script_class; }

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
        static void UnregisterObject(MonoObject *managed_object);
        
        static bool IsNativeClass(MonoClass *native_class);
        static Type GetNativeClass(MonoClass *native_class);
        static void RegisterClass(Type native_class, const char *managed_namespace, const char *managed_name);

        static MonoScriptingType *GetOrCreateScriptingType(MonoClass *managed_class);
    private:
        void InitDebugger(const ScriptingSettings &settings);
        void InitDomain();
        void InitBindings();
        void InitAssembly();
        void InitClasses();
        void InitMethods();

        void PrintUnhandledException(MonoObject *exception);
    private:
        inline static MonoDomain *s_core_domain;
        inline static MonoAssembly *s_core_assembly;
        inline static MonoImage *s_core_assembly_image;

        inline static MonoMethod *s_update_method;

        inline static Map<MonoObject *, void *> s_managed_to_native_objects;
        inline static Map<void *, MonoObject * > s_native_to_managed_objects;

        inline static Map<Type, MonoClass *> s_native_to_managed_classes;
        // NOTE: The variant just stores a Type.
        // We can not use Type directly because it does not have a public default constructor.
        inline static Map<MonoClass *, Variant> s_managed_to_native_classes; 

        inline static Map<MonoClass *, MonoScriptingType *> s_scripting_types;

        inline static MonoClass *s_component_class;
        inline static MonoClass *s_script_class;
    };

}