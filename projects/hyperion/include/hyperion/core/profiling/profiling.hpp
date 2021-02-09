#pragma once

#include <optick/optick.h>

#ifdef HYP_PROFILE
#define HYP_PROFILE_FRAME(name) OPTICK_FRAME(name)
#define HYP_PROFILE_THREAD(name) OPTICK_THREAD(name)
#define HYP_PROFILE_SCOPE(...) OPTICK_EVENT(__VA_ARGS__)
#define HYP_PROFILE_CATEGORY(name, category) OPTICK_CATEGORY(name, category)
#define HYP_PROFILE_TAG(name, ...) OPTICK_TAG(name, __VA_ARGS__)
#else
#define HYP_PROFILE_FRAME(name)
#define HYP_PROFILE_THREAD(name)
#define HYP_PROFILE_FUNCTION(...)
#define HYP_PROFILE_CATEGORY(name, category)
#endif