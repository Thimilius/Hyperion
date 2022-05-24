//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/ui/ui_immediate.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/assets/asset_manager.hpp"
#include "hyperion/assets/utilities/text_mesh_generator.hpp"
#include "hyperion/core/app/display.hpp"
#include "hyperion/render/render_engine.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::UI {

  //--------------------------------------------------------------
  void UIImmediate::Begin() {
    for (UIImmediateMesh mesh : s_meshes) {
      AssetManager::Unload(mesh.mesh);
    }
    s_meshes.Clear();

    UIImmediateLayout layout = { };
    layout.full_pivot = layout.leftover_pivot = Vector2(0.5f, 0.5f);
    layout.full_size = layout.leftover_size = Vector2(static_cast<float32>(Display::GetWidth()), static_cast<float32>(Display::GetHeight()));
    layout.full_position = layout.leftover_position = Vector3();
    s_layout_stack.Add(layout);
  }

  //--------------------------------------------------------------
  void UIImmediate::End() {
    Flush();
    
    Rendering::RenderFrameContext &render_frame_context = Rendering::RenderEngine::GetMainRenderFrame()->GetContext();
    for (UIImmediateMesh mesh : s_meshes) {
      Material *material = mesh.material ? mesh.material : AssetManager::GetMaterialPrimitive(MaterialPrimitive::UI);
      AssetId texture_id = mesh.texture
       ? mesh.texture->GetAssetInfo().id
       : AssetManager::GetTexture2DPrimitive(Texture2DPrimitive::White)->GetAssetInfo().id;
      
      Rendering::RenderFrameContextObjectUI &render_frame_context_ui_object = render_frame_context.AddEditorUIObject();
      render_frame_context_ui_object.local_to_world = Matrix4x4::Identity();
      render_frame_context_ui_object.mesh_id = mesh.mesh->GetAssetInfo().id;
      render_frame_context_ui_object.shader_id = material->GetShader()->GetAssetInfo().id;
      render_frame_context_ui_object.material_id = material->GetAssetInfo().id;
      render_frame_context_ui_object.color = mesh.color;
      render_frame_context_ui_object.texture.id = texture_id;
      render_frame_context_ui_object.texture.dimension = mesh.texture ? mesh.texture->GetDimension() : Rendering::TextureDimension::Texture2D;
      render_frame_context_ui_object.texture.render_texture_attchment_index = mesh.render_texture_attachment_index;
      render_frame_context_ui_object.enable_blending = mesh.enable_blending;
    }
  }

  //--------------------------------------------------------------
  void UIImmediate::BeginWindow(const String &name, DockingPosition docking_position, float32 extent, Color color, bool8 split) {
    Vector2 anchor_min = Vector2(0.5f, 0.5f);
    Vector2 anchor_max = Vector2(0.5f, 0.5f);
    Vector2 pivot = Vector2(0.5f, 0.5f);

    Vector2 size = Vector2(extent, extent);
    
    UIImmediateLayout parent_layout = s_layout_stack.GetLast();
    Vector2 parent_pivot = parent_layout.leftover_pivot;
    Vector2 parent_size = parent_layout.leftover_size;
    Vector2 half_parent_size = parent_size / 2.0f;

    Vector3 leftover_position = parent_layout.leftover_position;
    Vector2 leftover_size = parent_layout.leftover_size;

    switch (docking_position) {
      case DockingPosition::Full: {
        anchor_min = Vector2(0.0f, 0.0f);
        anchor_max = Vector2(1.0f, 1.0f);
        pivot = Vector2(0.5f, 0.5f);
        size = Vector2(0.0f, 0.0f);
        break;
      }
      case DockingPosition::Top: {
        anchor_min = Vector2(0.0f, 1.0f);
        anchor_max = Vector2(1.0f, 1.0f);
        pivot = Vector2(0.5f, 1.0f);
        size.x = 0.0f;
        extent = split ? half_parent_size.y : extent;
        size.y = extent;
        leftover_size.y -= extent;
        leftover_position.y -= extent / 2.0f;
        break;
      }
      case DockingPosition::Bottom: {
        anchor_min = Vector2(0.0f, 0.0f);
        anchor_max = Vector2(1.0f, 0.0f);
        pivot = Vector2(0.5f, 0.0f);
        size.x = 0.0f;
        extent = split ? half_parent_size.y : extent;
        size.y = extent;
        leftover_size.y -= extent;
        leftover_position.y += extent / 2.0f;
        break;
      }
      case DockingPosition::Left: {
        anchor_min = Vector2(0.0f, 0.0f);
        anchor_max = Vector2(0.0f, 1.0f);
        pivot = Vector2(0.0f, 0.5f);
        extent = split ? half_parent_size.x : extent;
        size.x = extent;
        size.y = 0.0f;
        leftover_size.x -= extent;
        leftover_position.x += extent / 2.0f;
        break;
      }
      case DockingPosition::Right: {
        anchor_min = Vector2(1.0f, 0.0f);
        anchor_max = Vector2(1.0f, 1.0f);
        pivot = Vector2(1.0f, 0.5f);
        extent = split ? half_parent_size.x : extent;
        size.x = extent;
        size.y = 0.0f;
        leftover_size.x -= extent;
        leftover_position.x -= extent / 2.0f;
        break;
      }
      default: HYP_ASSERT_ENUM_OUT_OF_RANGE; break;
    }

    float32 anchor_x = anchor_max.x - anchor_min.x;
    float32 anchor_y = anchor_max.y - anchor_min.y;
    float32 anchor_x_size = anchor_x * parent_size.x;
    float32 anchor_y_size = anchor_y * parent_size.y;
    size.x += anchor_x * parent_size.x;
    size.y += anchor_y * parent_size.y;
    Rect rect = Rect(-pivot * size, size);

    // First we have our local position which acts as a simple offset.
    Vector3 final_local_position = Vector3(0.0f, 0.0f, 0.0f);
    // We have to take into account the parent pivot which has an effect on our local position.
    final_local_position.x += (1.0f - parent_pivot.x) * parent_size.x - half_parent_size.x;
    final_local_position.y += (1.0f - parent_pivot.y) * parent_size.y - half_parent_size.y;
    // Now we need to take into account our own pivot.
    final_local_position.x += anchor_min.x * parent_size.x - half_parent_size.x;
    final_local_position.y += anchor_min.y * parent_size.y - half_parent_size.y;
    // Add the anchoring.
    final_local_position.x += pivot.x * anchor_x_size;
    final_local_position.y += pivot.y * anchor_y_size;

    Vector3 derived_position = final_local_position;
    derived_position += parent_layout.leftover_position;
    
    Matrix4x4 transform = Matrix4x4::Translate(derived_position);

    Vector2 min = rect.GetMin();
    Vector2 max = rect.GetMax();

    Vector3 corners[4];
    corners[0] = Vector3(max.x, max.y, 0.0f);
    corners[1] = Vector3(max.x, min.y, 0.0f);
    corners[2] = Vector3(min.x, min.y, 0.0f);
    corners[3] = Vector3(min.x, max.y, 0.0f);

    s_mesh_builder.AddQuad(corners, color);
    s_mesh_builder.Transform(transform);

    UIImmediateLayout &layout = s_layout_stack.GetLast();
    layout.leftover_size = leftover_size;
    layout.leftover_position = leftover_position;

    UIImmediateLayout new_layout = { };
    new_layout.full_size = new_layout.leftover_size = size;
    new_layout.full_pivot = new_layout.leftover_pivot = pivot;
    new_layout.full_position = new_layout.leftover_position = derived_position;
    s_layout_stack.Add(new_layout);
    
    Flush();
  }
  
  //--------------------------------------------------------------
  void UIImmediate::EndWindow() {
    s_layout_stack.RemoveLast();
  }

  //--------------------------------------------------------------
  void UIImmediate::DrawRect(Vector2 position, Vector2 size, Color color) {
    Flush();
    
    Vector2 half_size = size / 2.0f; 
    
    Vector3 corners[4];
    corners[0] = Vector3(position.x + half_size.x, position.y + half_size.y, 0.0f);
    corners[1] = Vector3(position.x + half_size.x, position.y - half_size.y, 0.0f);
    corners[2] = Vector3(position.x - half_size.x, position.y - half_size.y, 0.0f);
    corners[3] = Vector3(position.x - half_size.x, position.y + half_size.y, 0.0f);
    
    s_mesh_builder.AddQuad(corners, color);
  }
  
  //--------------------------------------------------------------
  void UIImmediate::DrawText(const String &text, Font *font, Vector2 position, UI::TextAlignment alignment, Color color) {
    Flush();

    Vector2 size = Vector2(1280, 720.0f);
    Vector2 half_size = size / 2.0f;

    TextMeshGenerationSettings generation_settings;
    generation_settings.text = text;
    generation_settings.font = font;
    generation_settings.alignment = alignment;
    generation_settings.color = color;
    generation_settings.rect = Rect(position - half_size, size);
    generation_settings.scale = Vector2(1.0f, 1.0f);

    TextMeshGenerator::GenerateMesh(generation_settings, s_mesh_builder);
    s_mesh_builder.TransformAndAlignPixels(Matrix4x4::Identity(), Vector2Int(Display::GetWidth(), Display::GetHeight()));

    Flush(AssetManager::GetMaterialPrimitive(MaterialPrimitive::Font), font->GetTexture());
  }
  
  //--------------------------------------------------------------
  void UIImmediate::Flush(Material *material, Texture *texture) {
    if (s_mesh_builder.IsEmpty()) {
      return;
    }
    
    UIImmediateMesh immediate_mesh = { };
    immediate_mesh.mesh = s_mesh_builder.CreateMesh();
    immediate_mesh.material = material ? material : AssetManager::GetMaterialPrimitive(MaterialPrimitive::UI);
    immediate_mesh.texture = texture ? texture : AssetManager::GetTexture2DPrimitive(Texture2DPrimitive::White);
    immediate_mesh.color = Color::White();
    immediate_mesh.render_texture_attachment_index = 0;
    immediate_mesh.enable_blending = true;
    s_meshes.Add(immediate_mesh);

    s_mesh_builder.Clear();
  }

}
