#pragma once

//---------------------- Library Includes ----------------------
#include <rttr/registration>
#include <rttr/registration_friend>
#include <rttr/type>

//---------------------- Project Includes ----------------------
#include "hyperion/core/object/metadata.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {
    using Instance = rttr::instance;
    template<typename T>
    using Registration = rttr::registration::class_<T>;
    using Type = rttr::type;
    using Variant = rttr::variant;

    const auto DefaultConstructorPolicy = rttr::policy::ctor::as_raw_ptr;
}

#define HYP_REFLECT(...) RTTR_ENABLE(__VA_ARGS__) \
        RTTR_REGISTRATION_FRIEND                  \
        public:
#define HYP_REFLECT_REGISTER_BEGIN RTTR_REGISTRATION { \
        using namespace rttr; \
        using namespace Hyperion;
#define HYP_REFLECT_REGISTER_END }