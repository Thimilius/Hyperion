#pragma once

// Standard library
#include <memory>
#include <string>
#include <vector>
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

#include "hyperion/common.hpp"

#include "hyperion/core/string_utils.hpp"
#include "hyperion/core/timer.hpp"

#include "hyperion/app/application.hpp"
#include "hyperion/app/window.hpp"
#include "hyperion/app/display.hpp"
#include "hyperion/app/input.hpp"
#include "hyperion/app/time.hpp"
#include "hyperion/app/events/events.hpp"

#include "hyperion/math/math.hpp"

#include "hyperion/rendering/render_api.hpp"
#include "hyperion/rendering/render_command.hpp"
#include "hyperion/rendering/buffer.hpp"
#include "hyperion/rendering/shader.hpp"

#include "hyperion/system/log.hpp"
#include "hyperion/system/engine.hpp"
#include "hyperion/system/operating_system.hpp"