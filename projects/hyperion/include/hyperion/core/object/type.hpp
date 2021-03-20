#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/common.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion {
    class MyType;
    struct MyTypeData;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion {

#define HYP_OBJECT()                                               \
    public:                                                        \
        inline MyType GetType() const { return s_type; }           \
    public:                                                        \
        inline static MyType *GetTypePointer() { return &s_type; } \
    private:                                                       \
        inline static MyTypeData *InitializeType();          \
    private:                                                       \
        inline static MyType s_type = InitializeType;              \
    private:                                                       \
        friend class Hyperion::MyType;                             \
    private:

#define HYP_OBJECT_REFLECT_BEGIN(type)                                                       \
    MyTypeData *type::InitializeType() {                                                     \
        static const std::unique_ptr<MyTypeData> type_data = std::make_unique<MyTypeData>(); \
        type_data->name = #type;
#define HYP_OBJECT_REFLECT_BASE(base_type) type_data->base = base_type::GetTypePointer();
#define HYP_OBJECT_REFLECT_END() return type_data.get(); \
    }

    typedef MyTypeData *(*TypeInitializerFunction)();

    struct MyTypeData final {
        String name;
        MyType *base = nullptr;
    };
    
    class MyType final {
    public:
        MyType() = default;
        MyType(TypeInitializerFunction initializer_function);
    public:
        bool IsValid() const { return m_data != nullptr; }

        bool IsDerivedFrom(MyType base);
        template<typename T> inline bool IsDerivedFrom() { return IsDerivedFrom(Get<T>()); }

        inline String GetName() const { return IsValid() ? m_data->name : "invalid"; }

        inline bool HasBase() const { return IsValid() ? m_data->base != nullptr : false; }
        inline MyType GetBase() const { return IsValid() ? (m_data->base != nullptr ? *m_data->base : MyType()) : MyType(); }

        inline bool operator==(const MyType &other) const { return m_data == other.m_data; }
        inline bool operator!=(const MyType &other) const { return !(*this == other); }
    public:
        template<typename T> inline static MyType Get() { return T::s_type; }
    private:
        MyTypeData *m_data = nullptr;
    private:
        friend struct std::hash<Hyperion::MyType>;
    };

}

namespace std {

    template <>
    struct hash<Hyperion::MyType> {
        std::size_t operator()(const Hyperion::MyType &type) const {
            return hash<void *>()(type.m_data);
        }
    };

}