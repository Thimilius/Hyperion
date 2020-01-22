#include "common.hpp"

// Core
#include "hyperion/core/types.hpp"
#include "hyperion/core/string_utils.hpp"
#include "hyperion/core/timer.hpp"
#include "hyperion/core/log.hpp"
#include "hyperion/core/engine.hpp"
#include "hyperion/core/operating_system.hpp"
#include "hyperion/core/color.hpp"
#include "hyperion/core/image.hpp"
#include "hyperion/core/io/file_utilities.hpp"
#include "hyperion/core/io/file_watcher.hpp"
#include "hyperion/core/io/image_loader.hpp"
#include "hyperion/core/math/math.hpp"
#include "hyperion/core/math/vec2.hpp"
#include "hyperion/core/math/vec3.hpp"
#include "hyperion/core/math/vec4.hpp"
#include "hyperion/core/math/mat4.hpp"
#include "hyperion/core/math/aabb.hpp"
#include "hyperion/core/math/ray.hpp"
#include "hyperion/core/math/rect.hpp"
#include "hyperion/core/app/application.hpp"
#include "hyperion/core/app/application_layer.hpp"
#include "hyperion/core/app/application_settings.hpp"
#include "hyperion/core/app/window.hpp"
#include "hyperion/core/app/display.hpp"
#include "hyperion/core/app/input.hpp"
#include "hyperion/core/app/time.hpp"
#include "hyperion/core/app/events/event.hpp"
#include "hyperion/core/app/events/app_events.hpp"
#include "hyperion/core/app/events/window_events.hpp"
#include "hyperion/core/app/events/key_events.hpp"
#include "hyperion/core/app/events/mouse_events.hpp"

// Entity
#include "hyperion/entity/object.hpp"
#include "hyperion/entity/world.hpp"
#include "hyperion/entity/entity.hpp"
#include "hyperion/entity/entity_event.hpp"
#include "hyperion/entity/components/entity_component.hpp"
#include "hyperion/entity/components/transform_component.hpp"
#include "hyperion/entity/components/mesh_renderer_component.hpp"

// Rendering
#include "hyperion/rendering/graphics_context.hpp"
#include "hyperion/rendering/render_engine.hpp"
#include "hyperion/rendering/renderer.hpp"
#include "hyperion/rendering/immediate_renderer.hpp"
#include "hyperion/rendering/shader.hpp"
#include "hyperion/rendering/material.hpp"
#include "hyperion/rendering/texture.hpp"
#include "hyperion/rendering/texture_2d.hpp"
#include "hyperion/rendering/texture_cubemap.hpp"
#include "hyperion/rendering/buffer.hpp"
#include "hyperion/rendering/vertex_array.hpp"
#include "hyperion/rendering/render_texture.hpp"
#include "hyperion/rendering/camera.hpp"
#include "hyperion/rendering/layer.hpp"

// Assets
#include "hyperion/assets/asset.hpp"
#include "hyperion/assets/asset_library.hpp"
#include "hyperion/assets/mesh_factory.hpp"

// Audio
#include "hyperion/audio/audio_engine.hpp"

// Editor
#include "hyperion/editor/editor_layer.hpp"
#include "hyperion/editor/editor_camera_controller.hpp"