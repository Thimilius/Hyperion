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
#include <fmt/format.h>
#include <stb/stb_image.h>
#include <fmod/fmod.hpp>

// Hyperion
#include "hyperion/common.hpp"
#include "hyperion/core/string_utils.hpp"
#include "hyperion/core/math/math.hpp"
#include "hyperion/core/operating_system.hpp"
#include "hyperion/core/engine.hpp"
#include "hyperion/core/math/math.hpp"
#include "hyperion/core/math/aabb.hpp"
#include "hyperion/core/math/vec2.hpp"
#include "hyperion/core/math/vec3.hpp"
#include "hyperion/core/math/vec4.hpp"
#include "hyperion/core/math/mat4.hpp"
#include "hyperion/rendering/render_api.hpp"