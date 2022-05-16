//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Library Includes ----------------------
#include <nameof/nameof.hpp>

#define HYP_NAME_OF(...) NAMEOF(__VA_ARGS__)
#define HYP_NAME_OF_TYPE(...) NAMEOF_SHORT_TYPE(__VA_ARGS__)
