#pragma once

#include <rttr/registration_friend>
#include <rttr/type>

#define HYP_REFLECT(...) RTTR_ENABLE(__VA_ARGS__) \
        RTTR_REGISTRATION_FRIEND                  \
        public:
#define HYP_REFLECT_REGISTER_BEGIN RTTR_REGISTRATION { \
        using namespace rttr; \
        using namespace Hyperion;
#define HYP_REFLECT_REGISTER_END }