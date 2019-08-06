#pragma once

#include <string>
#include <vector>
#include <array>

#ifdef HYP_PLATFORM_WINDOWS
    #include <Windows.h>
#endif

#include <glad/glad.h>
#include <glad/glad_wgl.h>
#include <stb/stb_image.h>

#include "hyperion/common.hpp"

#include "hyperion/core/string.hpp"
#include "hyperion/core/timer.hpp"

#include "hyperion/app/application.hpp"
#include "hyperion/app/window.hpp"
#include "hyperion/app/time.hpp"

#include "hyperion/math/math.hpp"

#include "hyperion/system/log.hpp"
#include "hyperion/system/engine.hpp"
#include "hyperion/system/operating_system.hpp"