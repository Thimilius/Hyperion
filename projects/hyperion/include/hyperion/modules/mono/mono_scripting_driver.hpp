#pragma once

#include <mono/jit/jit.h>

#include "hyperion/core/object/object.hpp"
#include "hyperion/scripting/scripting_driver.hpp"

namespace Hyperion::Scripting {

    class MonoScriptingDriver : public IScriptingDriver {
    public:
        void Initialize(const ScriptingSettings &settings) override;
        void Update() override;
        void Shutdown() override;
    public:
        inline static MonoDomain *GetDomain() { return s_core_domain; }

        template<typename T>
        static T *GetNativeObjectAs(MonoObject *managed_object) {
            return static_cast<T *>(GetNativeObject(managed_object));
        }
        static Object *GetNativeObject(MonoObject *managed_object);
        static MonoObject *GetManagedObject(Object *native_object);
        static MonoObject *GetOrCreateManagedObject(Object *native_object, Type native_type);
        static MonoObject *CreateManagedObject(Type type, Object *native_object);
        static bool IsRegisterdObject(MonoObject *managed_object);
        static void RegisterObject(MonoObject *managed_object, Object *native_object);
        static void UnregisterObject(MonoObject *managed_object);
        
        static Type GetNativeClass(MonoClass *native_class);
        static void RegisterClass(Type native_class, const char *managed_namespace, const char *managed_name);
    private:
        void InitDebugger(const ScriptingSettings &settings);
        void InitDomain();
        void InitBindings();
        void InitAssembly();
        void InitMethods();

        void PrintUnhandledException(MonoObject *exception);
    private:
        inline static MonoDomain *s_core_domain;
        inline static MonoAssembly *s_core_assembly;
        inline static MonoImage *s_core_assembly_image;

        inline static MonoMethod *s_update_method;

        inline static Map<MonoObject *, Object *> s_native_objects;
        inline static Map<Object *, MonoObject *> s_managed_objects;
        inline static Map<Type, MonoClass *> s_managed_classes;
        // NOTE: The variant just stores a Type.
        // We can not use Type directly because it does not have a public default constructor.
        inline static Map<MonoClass *, Variant> s_native_classes; 
    };

}