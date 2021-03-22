#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/common.hpp"
#include "hyperion/core/object/non_copyable.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion {
    class Type;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    enum class MetadataKind {
        Invalid,

        RequireComponent,
        DisallowMultipleComponent,
    };

    struct Metadata {
        MetadataKind kind = MetadataKind::Invalid;
        union MetadataData {
            bool boolean;
            void *opaque_pointer;
            Type *type_pointer;
        } data;
    };

    using TypeConstructorFunction = std::function<void*()>;
    
    struct TypeData final {
        String name;
        Type *base = nullptr;
        TypeConstructorFunction constructor = nullptr;
        Vector<Metadata> metadata;
    };

    typedef TypeData *(*TypeInitializerFunction)();
    
    class Type final : public INonCopyable {
    public:
        Type(TypeInitializerFunction initializer_function);
    public:
        bool IsDerivedFrom(Type *base);
        template<typename T> inline bool IsDerivedFrom() { return IsDerivedFrom(Get<T>()); }

        inline String GetName() const { return m_data->name; }

        inline bool HasBase() const { return m_data->base != nullptr; }
        inline Type *GetBase() const { return m_data->base; }

        const Vector<Metadata> &GetMetadata() const { return m_data->metadata; }

        inline bool HasConstructor() const { return m_data->constructor != nullptr; }
        inline void *Create() { HYP_ASSERT(HasConstructor()); return m_data->constructor(); }
        template<typename T> inline T *CreateAs() const { HYP_ASSERT(HasConstructor()); return reinterpret_cast<T *>(m_data->constructor()); }

        inline bool operator==(const Type *other) const { return m_data == other->m_data; }
        inline bool operator!=(const Type *other) const { return !(*this == other); }
    public:
        template<typename T> inline static Type *Get() { return T::GetStaticType(); }
        static Type *GetByName(const String &name);
    private:
        TypeData *m_data = nullptr;
    private:
        inline static Vector<Type *> s_types;
    };

}

#define HYP_REFLECT()                                                      \
    public:                                                                \
        inline virtual Hyperion::Type *GetType() const { return &s_type; } \
    public:                                                                \
        inline static Hyperion::Type *GetStaticType() { return &s_type; }  \
    private:                                                               \
        static Hyperion::TypeData *InitializeType();                       \
    private:                                                               \
        inline static Hyperion::Type s_type = InitializeType;              \
    private:

#define HYP_REFLECT_BEGIN(type)                                                                              \
    Hyperion::TypeData *type::InitializeType() {                                                             \
        static const std::unique_ptr<Hyperion::TypeData> type_data = std::make_unique<Hyperion::TypeData>(); \
        type_data->name = #type;
#define HYP_REFLECT_BASE(base_type) type_data->base = base_type::GetStaticType();
#define HYP_REFLECT_CONSTRUCTOR(constructor_function) type_data->constructor = constructor_function;
#define HYP_REFLECT_METADATA_POINTER(k, p) {     \
        Hyperion::Metadata metadata;             \
        metadata.kind = k;                       \
        metadata.data.opaque_pointer = p;        \
        type_data->metadata.push_back(metadata); \
    }
#define HYP_REFLECT_END() return type_data.get(); }