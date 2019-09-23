#include "common.hpp"

#include "core/types.hpp"
#include "core/string_utils.hpp"
#include "core/timer.hpp"

#include "app/application.hpp"
#include "app/window.hpp"
#include "app/display.hpp"
#include "app/input.hpp"
#include "app/time.hpp"
#include "app/events/event.hpp"

#include "entity/game_object.hpp"
#include "entity/camera_controller.hpp"
#include "entity/components/component.hpp"
#include "entity/components/transform_component.hpp"

#include "math/math.hpp"

#include "rendering/graphics_context.hpp"
#include "rendering/render_command.hpp"
#include "rendering/renderer.hpp"
#include "rendering/shader.hpp"
#include "rendering/texture.hpp"
#include "rendering/buffer.hpp"
#include "rendering/vertex_array.hpp"
#include "rendering/render_texture.hpp"
#include "rendering/camera.hpp"

#include "io/file_utilities.hpp"
#include "io/file_watcher.hpp"

#include "system/log.hpp"
#include "system/engine.hpp"
#include "system/operating_system.hpp"

// ------------ ENTRY POINT --------------
#include "entry_point.hpp"
// ---------------------------------------