#include "common.hpp"

// App
#include "app/application.hpp"
#include "app/window.hpp"
#include "app/display.hpp"
#include "app/input.hpp"
#include "app/time.hpp"
#include "app/events/event.hpp"

// Audio
#include "audio/audio_engine.hpp"

// Core
#include "core/types.hpp"
#include "core/string_utils.hpp"
#include "core/timer.hpp"

// Entity
#include "entity/game_object.hpp"
#include "entity/camera_controller.hpp"
#include "entity/components/component.hpp"
#include "entity/components/transform_component.hpp"

// IO
#include "io/file_utilities.hpp"
#include "io/file_watcher.hpp"

// Math
#include "math/math.hpp"

// Rendering
#include "rendering/graphics_context.hpp"
#include "rendering/render_command.hpp"
#include "rendering/renderer.hpp"
#include "rendering/immediate_renderer.hpp"
#include "rendering/shader.hpp"
#include "rendering/texture.hpp"
#include "rendering/buffer.hpp"
#include "rendering/vertex_array.hpp"
#include "rendering/render_texture.hpp"
#include "rendering/camera.hpp"
#include "rendering/layer.hpp"

// System
#include "system/log.hpp"
#include "system/engine.hpp"
#include "system/operating_system.hpp"