#ifndef HYP_PCH
#define HYP_PCH

// C standard library
#include <cstdint>
#include <cfloat>
#include <cmath>
#include <ctime>

// C++ standard library
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <functional>
#include <filesystem>

// Platform specific
#ifdef HYP_PLATFORM_WINDOWS
    #include <Windows.h>
#endif

// Vendors
#include <fmt/format.h>

// Hyperion
#include "hyperion/common.hpp"
#include "hyperion/core/string_utils.hpp"
#include "hyperion/core/math/math.hpp"
#include "hyperion/core/operating_system.hpp"
#include "hyperion/core/engine.hpp"
#include "hyperion/core/math/math.hpp"
#include "hyperion/core/math/vec2.hpp"
#include "hyperion/core/math/vec3.hpp"
#include "hyperion/core/math/vec4.hpp"
#include "hyperion/core/math/mat3.hpp"
#include "hyperion/core/math/mat4.hpp"
#include "hyperion/core/math/aabb.hpp"
#include "hyperion/core/math/plane.hpp"
#include "hyperion/core/math/ray.hpp"
#include "hyperion/core/math/rect.hpp"
#include "hyperion/rendering/render_engine.hpp"

#endif
