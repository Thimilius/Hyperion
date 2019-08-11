#include "common.hpp"

#include "core/string.hpp"
#include "core/list.hpp"

#include "app/application.hpp"
#include "app/window.hpp"
#include "app/display.hpp"
#include "app/input.hpp"
#include "app/time.hpp"
#include "app/events/event.hpp"

#include "entity/game_object.hpp"
#include "entity/components/component.hpp"
#include "entity/components/transform_component.hpp"

#include "math/math.hpp"

#include "rendering/render_command.hpp"
#include "rendering/buffer.hpp"

#include "system/log.hpp"
#include "system/engine.hpp"
#include "system/operating_system.hpp"

// ------------ ENTRY POINT --------------
#include "entry_point.hpp"
// ---------------------------------------