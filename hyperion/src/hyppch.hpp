#pragma once

// Standard library
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <functional>

// Platform specific
#ifdef HYP_PLATFORM_WINDOWS
    #include <Windows.h>
#endif

// Vendors
#include <glad/glad.h>
#include <glad/glad_wgl.h>
#include <vendor/fmt/format.h>
#include <stb/stb_image.h>
#include <fmod/fmod.hpp>

// Hyperion
#include "hyperion/common.hpp"
#include "hyperion/core/string_utils.hpp"
#include "hyperion/math/math.hpp"
#include "hyperion/rendering/render_api.hpp"
#include "hyperion/system/operating_system.hpp"