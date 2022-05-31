//--------------------- Definition Include ---------------------
#include "sandbox/sandbox_application.hpp"

//---------------------- Library Includes ----------------------
#include <hyperion/entry_point.hpp>
#include <hyperion/assets/asset_manager.hpp>
#include <hyperion/assets/material.hpp>
#include <hyperion/assets/loader/font_loader.hpp>
#include <hyperion/assets/loader/image_loader.hpp>
#include <hyperion/assets/loader/mesh_loader.hpp>
#include <hyperion/assets/utilities/mesh_generator.hpp>
#include <hyperion/core/random.hpp>
#include <hyperion/core/app/display.hpp>
#include <hyperion/core/app/time.hpp>
#include <hyperion/core/io/file_system.hpp>
#include <hyperion/core/memory/memory.hpp>
#include <hyperion/ecs/component/components/components.hpp>
#include <hyperion/ecs/component/components/utilities/camera_controller.hpp>
#include <hyperion/ecs/component/components/utilities/camera_utilities.hpp>
#include <hyperion/ecs/component/components/utilities/transform_utilities.hpp>
#include <hyperion/ecs/world/world_manager.hpp>
#include <hyperion/ecs/world/world_serializer.hpp>
#include <hyperion/render/render_engine.hpp>
#include <hyperion/render/pipelines/forward/forward_render_pipeline.hpp>
#include <hyperion/ui/ui_button.hpp>
#include <hyperion/ui/ui_label.hpp>


//------------------------- Namespaces -------------------------
using namespace Hyperion;
using namespace Hyperion::Rendering;

#if HYP_RELEASE || HYP_PROFILE
//#define HYP_STRESS_TEST
//#define HYP_STRESS_TEST_EXTREME
#endif

//-------------------- Definition Namespace --------------------
namespace Sandbox {

  //--------------------------------------------------------------
  void SandboxApplication::OnSetup(ApplicationSettings &settings) {
    settings.render.backend = RenderBackend::OpenGL;
    settings.render.threading_mode = RenderThreadingMode::MultiThreaded;
    settings.render.vsync_mode = VSyncMode::DontSync;
  }

  World *g_world;
  EntityId g_camera;
  EntityId g_light;
  EntityId g_parent;
  EntityId g_child;

  CameraController *g_camera_controller;

  //--------------------------------------------------------------
  void SandboxApplication::OnInitialize() {
    g_world = WorldManager::CreateWorld();
    WorldManager::SetActiveWorld(g_world);

    EntityManager *manager = g_world->GetEntityManager();
    
    g_camera = manager->CreateEntity(EntityPrimitive::Camera);
    g_light = manager->CreateEntity(EntityPrimitive::DirectionalLight);

    g_camera_controller = new LookAroundCameraController(g_camera);
    g_camera_controller->Reset(manager);
    LocalTransformComponent *camera_transform = manager->GetComponent<LocalTransformComponent>(g_camera);

    g_parent = manager->CreateEntity(EntityPrimitive::Sphere);
#ifdef HYP_STRESS_TEST
    float32 size = 100;
    for (float32 x = 0; x < size; x++) {
      for (float32 z = 0; z < size; z++) {
#ifdef HYP_STRESS_TEST_EXTREME
        Material *material = AssetManager::CreateMaterial(AssetManager::GetShaderPrimitive(ShaderPrimitive::Standard));
        material->SetColor("m_color", Color(Random::Get(), Random::Get(), Random::Get(), 1.0f));
#endif
        EntityId entity = manager->CreateEntity(EntityPrimitive::Cube);
        manager->GetComponent<LocalTransformComponent>(entity)->position = Vector3(x * 2.0f, 0.0f, -z * 2.0f);
#ifdef HYP_STRESS_TEST_EXTREME
        manager->GetComponent<MeshComponent>(entity)->material = material;
#endif
        g_world->GetHierarchy()->SetParent(entity, g_parent);
        manager->RemoveComponent<Physics::BoxColliderComponent>(entity);
      }
    }

    for (uint32 i = 0; i < 128; i++) {
      float32 x = Random::Get() * size * 2.0f;
      float32 z = Random::Get() * size * 2.0f;

      EntityId point_light = manager->CreateEntity(EntityPrimitive::PointLight);
      manager->GetComponent<LocalTransformComponent>(point_light)->position = Vector3(x, 1.0f, -z);
      manager->GetComponent<PointLightComponent>(point_light)->color = Color(Random::Get(), Random::Get(), Random::Get(), 1.0f);
      manager->GetComponent<PointLightComponent>(point_light)->intensity = 5.0f;
    }
#else
    g_child = manager->CreateEntity(EntityPrimitive::Cube);
    manager->GetComponent<LocalTransformComponent>(g_child)->position = Vector3(2.0f, 0.0f, 0.0f);
    g_world->GetHierarchy()->SetParent(g_child, g_parent);

    std::unique_ptr<Image> image;
    image.reset(ImageLoader::Load("icon/icon.png").Unwrap());
    Texture2DParameters parameters;
    parameters.format = TextureFormat::RGBA32;
    parameters.width = image->GetWidth();
    parameters.height = image->GetHeight();
    Texture2D *texture = AssetManager::CreateTexture2D(parameters, image->GetPixels());
    Material *material = AssetManager::CreateMaterial(AssetManager::GetShaderPrimitive(ShaderPrimitive::Unlit));
    material->SetTexture("m_texture", texture);
    EntityId quad = manager->CreateEntity(EntityPrimitive::Quad);
    manager->GetComponent<MeshComponent>(quad)->material = material;
    manager->GetComponent<LocalTransformComponent>(quad)->position = Vector3(0.0f, 2.0f, 0.0f);
#endif
    
    UpdateTitle();
  }

  //--------------------------------------------------------------
  void SandboxApplication::OnUpdate(float32 delta_time) {
    if (Input::IsKeyHold(KeyCode::Control) && Input::IsKeyDown(KeyCode::W)) {
      Exit();
    }
    if (Input::IsKeyDown(KeyCode::F1)) {
      GetMainWindow()->SetWindowMode(GetMainWindow()->GetWindowMode() == WindowMode::Borderless ? WindowMode::Windowed : WindowMode::Borderless);
    }
    if (Input::IsKeyDown(KeyCode::F2)) {
      Rendering::RenderEngine::SetVSyncMode(
        Rendering::RenderEngine::GetVSyncMode() == Rendering::VSyncMode::DontSync ? Rendering::VSyncMode::EveryVBlank : Rendering::VSyncMode::DontSync);
    }
    if (Input::IsKeyDown(KeyCode::F3)) {
      Rendering::RenderGizmos::SetShouldDrawGrid(!Rendering::RenderGizmos::GetShouldDrawGrid());
    }
    if (Input::IsKeyDown(KeyCode::F4)) {
      Rendering::RenderGizmos::SetShouldDrawAllBounds(!Rendering::RenderGizmos::GetShouldDrawAllBounds());
    }

    EntityManager *manager = g_world->GetEntityManager();
    
    g_camera_controller->Update(manager, delta_time);
    if (Input::IsKeyDown(KeyCode::R)) {
      g_camera_controller->Reset(manager);
    }

    manager->GetComponent<LocalTransformComponent>(g_parent)->rotation = Quaternion::FromEulerAngles(0.0f, Math::Sin(Time::GetTime()) * 45.0f, 0.0f);

#ifdef HYP_STRESS_TEST
    Quaternion rotation = Quaternion::FromEulerAngles(0.0f, Time::GetTime() * 25.0f, 0.0f);
    auto view = manager->GetView<LocalTransformComponent, MeshComponent>(ExcludeComponents<Physics::SphereColliderComponent>());
    for (EntityId entity : view) {
      LocalTransformComponent *transform = manager->GetComponent<LocalTransformComponent>(entity);
      transform->rotation = rotation;
#ifdef HYP_STRESS_TEST_EXTREME
      MeshComponent *mesh = manager->GetComponent<MeshComponent>(entity);
      mesh->material->SetColor("m_color", Color(Random::Get(), Random::Get(), Random::Get(), 1.0f));
#endif
    }
#endif
    UpdateTitle();

    auto get_closest_t_for_first_ray = [](Ray l1, Ray l2) -> float {
      const Vector3 dp = l2.origin - l1.origin;
      const float v12 = l1.direction.Dot(l1.direction);
      const float v22 = l2.direction.Dot(l2.direction);
      const float v1v2 = l1.direction.Dot(l2.direction);

      const float det = v1v2 * v1v2 - v12 * v22;

      if (std::abs(det) > FLT_MIN)
      {
        const float inv_det = 1.f / det;

        const float dpv1 = dp.Dot(l1.direction);
        const float dpv2 = dp.Dot(l2.direction);

        auto l1t = inv_det * (v22 * dpv1 - v1v2 * dpv2);
        auto l2t = inv_det * (v1v2 * dpv1 - v12 * dpv2);
        
        return -l1t;
      }
      else
      {
        const Vector3 a = Vector3::Cross(dp, l1.direction);
        return std::sqrt(a.Dot(a) / v12);
      }
    };

    auto distance_between_rays = [](Ray a, Ray b) {
      Vector3 cross = Vector3::Cross(a.direction, b.direction);
      float32 mag = cross.Magnitude();
      if (mag == 0.0f) {
        return 0.0f;
      }

      Vector3 p = b.origin - a.origin;
      float32 d = cross.Dot(p);

      return d / mag;
    };
    
    CameraComponent *camera = manager->GetComponent<CameraComponent>(g_camera);
    DerivedTransformComponent *camera_transform = manager->GetComponent<DerivedTransformComponent>(g_camera);
    Ray ray = CameraUtilities::ScreenPointToRay(camera, camera_transform, Input::GetMousePosition().ToFloat());

    Material *material = AssetManager::GetMaterialPrimitive(MaterialPrimitive::Default);
    LocalTransformComponent *transform = manager->GetComponent<LocalTransformComponent>(g_parent);
    
    static Vector3 offset = Vector3();
    static bool8 should_move_x = false;
    static bool8 should_move_y = false;
    static bool8 should_move_z = false;
    
    Vector3 position = transform->position;
    Ray x_axis_ray = Ray(Vector3(0.0f, position.y, position.z), Vector3(1, 0, 0));
    Ray y_axis_ray = Ray(Vector3(position.x, 0.0f, position.z), Vector3(0, 1, 0));
    Ray z_axis_ray = Ray(Vector3(position.x, position.y, 0.0f), Vector3(0, 0, 1));
    Vector3 x_handle_position = Vector3(position.x + 0.5f, position.y, position.z);
    Vector3 y_handle_position = Vector3(position.x, position.y + 0.5f, position.z);
    Vector3 z_handle_position = Vector3(position.x, position.y, position.z + 0.5f);

    float32 distance_to_x = distance_between_rays(x_axis_ray, ray);
    float32 x_t = get_closest_t_for_first_ray(x_axis_ray, ray);
    Vector3 x_axis_point = x_axis_ray.GetPoint(x_t);
    float32 distance_to_y = distance_between_rays(y_axis_ray, ray);
    float32 y_t = get_closest_t_for_first_ray(y_axis_ray, ray);
    Vector3 y_axis_point = y_axis_ray.GetPoint(y_t);
    float32 distance_to_z = distance_between_rays(z_axis_ray, ray);
    float32 z_t = get_closest_t_for_first_ray(z_axis_ray, ray);
    Vector3 z_axis_point = z_axis_ray.GetPoint(z_t);
    
    material->SetColor("m_color", Color::White());

    bool8 near_x = x_handle_position.Distance(x_axis_point) < 0.5f && std::abs(distance_to_x) < 0.1f;
    bool8 near_y = y_handle_position.Distance(y_axis_point) < 0.5f && std::abs(distance_to_y) < 0.1f;
    bool8 near_z = z_handle_position.Distance(z_axis_point) < 0.5f && std::abs(distance_to_z) < 0.1f;

    Plane xy_plane = Plane(Vector3(0.0f, 0.0f, 1.0f), -position.z);
    Plane xz_plane = Plane(Vector3(0.0f, 1.0f, 0.0f), -position.y); 
    Plane yz_plane = Plane(Vector3(1.0f, 0.0f, 0.0f), -position.x);

    Vector3 camera_direction = TransformUtilities::GetForward(camera_transform);
    float32 xy_plane_dot = Math::Abs(xy_plane.normal.Dot(camera_direction));
    float32 xz_plane_dot = Math::Abs(xz_plane.normal.Dot(camera_direction));
    float32 yz_plane_dot = Math::Abs(yz_plane.normal.Dot(camera_direction));
    
    if (near_x) {
      if (Input::IsMouseButtonDown(MouseButtonCode::Left)) {
        offset = transform->position - x_axis_point;
        should_move_x = true;
      }
      material->SetColor("m_color", Color::Red());
    }
    if (near_y) {
      if (Input::IsMouseButtonDown(MouseButtonCode::Left)) {
        offset = transform->position - y_axis_point;
        should_move_y = true;
      }
      material->SetColor("m_color", Color::Green());
    }
    if (near_z) {
      if (Input::IsMouseButtonDown(MouseButtonCode::Left)) {
        offset = transform->position - z_axis_point;
        should_move_z = true;
      }
      material->SetColor("m_color", Color::Blue());
    }

    Plane plane;
    float32 plane_distance = 0.0f;
    if (Input::IsMouseButtonHold(MouseButtonCode::Left)) {
      if (should_move_x) {
        if (xy_plane_dot > xz_plane_dot) {
          plane = xy_plane;
        } else {
          plane = xz_plane;
        }
        plane.Intersects(ray, plane_distance);
        x_axis_point.x = ray.GetPoint(plane_distance).x;
        
        transform->position = x_axis_point + offset;
        material->SetColor("m_color", Color::Red());  
      }
      if (should_move_y) {
        if (xy_plane_dot > yz_plane_dot) {
          plane = xy_plane;
        } else {
          plane = yz_plane;
        }
        plane.Intersects(ray, plane_distance);
        y_axis_point.y = ray.GetPoint(plane_distance).y;
        
        transform->position = y_axis_point + offset;
        material->SetColor("m_color", Color::Green());  
      }
      if (should_move_z) {
        if (xz_plane_dot > yz_plane_dot) {
          plane = xz_plane;
        } else {
          plane = yz_plane;
        }
        plane.Intersects(ray, plane_distance);
        z_axis_point.z = ray.GetPoint(plane_distance).z;
        
        transform->position = z_axis_point + offset;
        material->SetColor("m_color", Color::Blue());  
      }
    }

    if (Input::IsMouseButtonUp(MouseButtonCode::Left)) {
      should_move_x = false;
      should_move_y = false;
      should_move_z = false;
    }
  }

  //--------------------------------------------------------------
  void SandboxApplication::OnTick() { }

  //--------------------------------------------------------------
  void SandboxApplication::UpdateTitle() {
    String format = "Sandbox - FPS: {} ({:.2f}ms) - VSync: {} - Draw calls: {}, Vertices: {}, Triangles: {} - Memory: {}";
    RenderStats render_stats = Rendering::RenderEngine::GetStats();
    String vsync = Rendering::RenderEngine::GetVSyncMode() == Rendering::VSyncMode::DontSync ? "Off" : "On";
    uint64 memory = MemoryStats::GetGlobalMemory();
    String title = StringUtils::Format(format,
                                       Time::GetFPS(),
                                       Time::GetFrameTime(),
                                       vsync,
                                       render_stats.draw_calls,
                                       render_stats.vertex_count,
                                       render_stats.triangle_count,
                                       memory);

    GetMainWindow()->SetTitle(title);
  }

}

//--------------------------------------------------------------
Application *Hyperion::CreateApplication() {
  return new Sandbox::SandboxApplication();
}
