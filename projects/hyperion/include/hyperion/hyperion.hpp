#include "hyperion/common.hpp"

// Core
#include "hyperion/core/color.hpp"
#include "hyperion/core/engine.hpp"
#include "hyperion/core/image.hpp"
#include "hyperion/core/log.hpp"
#include "hyperion/core/object.hpp"
#include "hyperion/core/result.hpp"
#include "hyperion/core/string_utils.hpp"
#include "hyperion/core/timer.hpp"
#include "hyperion/core/types.hpp"
#include "hyperion/core/app/application.hpp"
#include "hyperion/core/app/application_settings.hpp"
#include "hyperion/core/app/display.hpp"
#include "hyperion/core/app/input.hpp"
#include "hyperion/core/app/time.hpp"
#include "hyperion/core/app/window.hpp"
#include "hyperion/core/io/file_utilities.hpp"
#include "hyperion/core/io/image_loader.hpp"
#include "hyperion/core/math/bounding_box.hpp"
#include "hyperion/core/math/bounding_sphere.hpp"
#include "hyperion/core/math/math.hpp"
#include "hyperion/core/math/mat3.hpp"
#include "hyperion/core/math/mat4.hpp"
#include "hyperion/core/math/plane.hpp"
#include "hyperion/core/math/ray.hpp"
#include "hyperion/core/math/rect.hpp"
#include "hyperion/core/math/vec2.hpp"
#include "hyperion/core/math/vec3.hpp"
#include "hyperion/core/math/vec4.hpp"

// Entity
#include "hyperion/entity/entity.hpp"
#include "hyperion/entity/world.hpp"
#include "hyperion/entity/world_manager.hpp"
#include "hyperion/entity/components/transform.hpp"
#include "hyperion/entity/components/physics/box_collider.hpp"
#include "hyperion/entity/components/physics/sphere_collider.hpp"
#include "hyperion/entity/components/rendering/camera.hpp"
#include "hyperion/entity/components/rendering/light.hpp"
#include "hyperion/entity/components/rendering/mesh_renderer.hpp"

// Rendering
#include "hyperion/rendering/buffer.hpp"
#include "hyperion/rendering/camera_data.hpp"
#include "hyperion/rendering/material.hpp"
#include "hyperion/rendering/render_texture.hpp"
#include "hyperion/rendering/shader.hpp"
#include "hyperion/rendering/texture.hpp"
#include "hyperion/rendering/texture_2d.hpp"
#include "hyperion/rendering/texture_cubemap.hpp"
#include "hyperion/rendering/vertex_array.hpp"

// Assets
#include "hyperion/assets/asset.hpp"
#include "hyperion/assets/asset_manager.hpp"
#include "hyperion/assets/mesh_factory.hpp"

// Physics
#include "hyperion/physics/physics_engine.hpp"

// Audio
#include "hyperion/audio/audio_engine.hpp"