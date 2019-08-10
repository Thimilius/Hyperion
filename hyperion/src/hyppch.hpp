#pragma once

#ifdef HYP_PLATFORM_WINDOWS
    #include <Windows.h>
#endif

#include <glad/glad.h>
#include <glad/glad_wgl.h>
#include <vendor/fmt/format.h>
#include <stb/stb_image.h>

#include "hyperion/common.hpp"

#include "hyperion/core/string.hpp"
#include "hyperion/core/timer.hpp"

#include "hyperion/app/application.hpp"
#include "hyperion/app/window.hpp"
#include "hyperion/app/display.hpp"
#include "hyperion/app/input.hpp"
#include "hyperion/app/time.hpp"
#include "hyperion/app/events/events.hpp"

#include "hyperion/entity/game_object.hpp"
#include "hyperion/entity/components/component.hpp"
#include "hyperion/entity/components/transform_component.hpp"

#include "hyperion/math/math.hpp"

#include "hyperion/rendering/render_api.hpp"
#include "hyperion/rendering/render_command.hpp"

#include "hyperion/system/log.hpp"
#include "hyperion/system/engine.hpp"
#include "hyperion/system/operating_system.hpp"