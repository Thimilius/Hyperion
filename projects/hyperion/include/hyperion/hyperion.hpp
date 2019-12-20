#include "common.hpp"

// Core
#include "hyperion/core/types.hpp"
#include "hyperion/core/string_utils.hpp"
#include "hyperion/core/timer.hpp"
#include "hyperion/core/log.hpp"
#include "hyperion/core/engine.hpp"
#include "hyperion/core/operating_system.hpp"
#include "hyperion/core/io/file_utilities.hpp"
#include "hyperion/core/io/file_watcher.hpp"
#include "hyperion/core/math/math.hpp"
#include "hyperion/core/math/bounds.hpp"
#include "hyperion/core/math/vec2.hpp"
#include "hyperion/core/math/vec3.hpp"
#include "hyperion/core/math/vec4.hpp"
#include "hyperion/core/math/mat4.hpp"

// App
#include "hyperion/app/application.hpp"
#include "hyperion/app/window.hpp"
#include "hyperion/app/display.hpp"
#include "hyperion/app/input.hpp"
#include "hyperion/app/time.hpp"
#include "hyperion/app/events/event.hpp"

// Rendering
#include "hyperion/rendering/graphics_context.hpp"
#include "hyperion/rendering/render_command.hpp"
#include "hyperion/rendering/renderer.hpp"
#include "hyperion/rendering/immediate_renderer.hpp"
#include "hyperion/rendering/shader.hpp"
#include "hyperion/rendering/texture.hpp"
#include "hyperion/rendering/buffer.hpp"
#include "hyperion/rendering/vertex_array.hpp"
#include "hyperion/rendering/render_texture.hpp"
#include "hyperion/rendering/camera.hpp"
#include "hyperion/rendering/layer.hpp"

// Audio
#include "hyperion/audio/audio_engine.hpp"

// Entity
#include "hyperion/entity/camera_controller.hpp"