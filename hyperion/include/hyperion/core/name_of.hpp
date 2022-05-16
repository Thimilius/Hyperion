//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Library Includes ----------------------
#include <nameof/nameof.hpp>

//---------------------- Project Includes ----------------------
#include "hyperion/core/string_utils.hpp"

#define HYP_NAME_OF(...) String(NAMEOF(__VA_ARGS__))
#define HYP_NAME_OF_TYPE(...) String(NAMEOF_SHORT_TYPE(__VA_ARGS__))
#define HYP_NAME_OF_ENUM(...) String(NAMEOF_ENUM_CONST(__VA_ARGS__))
