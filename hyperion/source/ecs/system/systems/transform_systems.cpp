//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/ecs/system/systems/transform_systems.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/ecs/component/components/core_components.hpp"
#include "hyperion/ecs/world/world.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  //--------------------------------------------------------------
  void HierarchyTransformSystem::Run(EntityManager *manager) {
    HYP_PROFILE_SCOPE("HierarchyTransformSystem.Run");

    EntityId root = manager->GetWorld()->GetHierarchy()->GetFirstRoot();
    uint64 root_count = manager->GetWorld()->GetHierarchy()->GetRootCount();
    for (uint64 i = 0; i < root_count; i++) {
      HierarchyComponent *root_hierarchy = manager->GetComponent<HierarchyComponent>(root);
      bool8 is_not_disabled = manager->GetComponent<DisabledComponent>(root) == nullptr;
      bool8 is_not_static = manager->GetComponent<StaticComponent>(root) == nullptr;
      if (is_not_disabled && is_not_static) {
        UpdateBranch(manager, root, manager->GetComponent<HierarchyComponent>(root), nullptr);
      }
      root = root_hierarchy->next_sibling;
    }
  }

  //--------------------------------------------------------------
  void HierarchyTransformSystem::UpdateBranch(EntityManager *manager, EntityId branch, HierarchyComponent *branch_hierarchy,
                                              DerivedTransformComponent *parent_derived_transform) {
    HYP_PROFILE_SCOPE("HierarchyTransformSystem.UpdateBranch");

    LocalTransformComponent *local_transform = manager->GetComponent<LocalTransformComponent>(branch);
    DerivedTransformComponent *derived_transform = manager->GetComponent<DerivedTransformComponent>(branch);

    if (parent_derived_transform == nullptr) {
      derived_transform->position = local_transform->position;
      derived_transform->rotation = local_transform->rotation;
      derived_transform->scale = local_transform->scale;
    } else {
      derived_transform->position = parent_derived_transform->rotation * (parent_derived_transform->scale * local_transform->position);
      derived_transform->position += parent_derived_transform->position;
      derived_transform->rotation = parent_derived_transform->rotation * local_transform->rotation;
      derived_transform->scale = parent_derived_transform->scale * local_transform->scale;
    }

    EntityId child = branch_hierarchy->first_child;
    for (uint64 i = 0; i < branch_hierarchy->child_count; i++) {
      HierarchyComponent *child_hierarchy = manager->GetComponent<HierarchyComponent>(child);
      bool8 is_not_disabled = manager->GetComponent<DisabledComponent>(child) == nullptr;
      bool8 is_not_static = manager->GetComponent<StaticComponent>(child) == nullptr;
      if (is_not_disabled && is_not_static) {
        UpdateBranch(manager, child, child_hierarchy, derived_transform);
      }
      child = child_hierarchy->next_sibling;
    }
  }

  //--------------------------------------------------------------
  void LocalToWorldSystem::Run(EntityManager *manager) {
    HYP_PROFILE_SCOPE("LocalToWorldSystem.Run");

    auto view = manager->GetView<DerivedTransformComponent, LocalToWorldComponent>();
    for (EntityId entity : view) {
      DerivedTransformComponent *derived_transform = manager->GetComponent<DerivedTransformComponent>(entity);
      LocalToWorldComponent *local_to_world = manager->GetComponent<LocalToWorldComponent>(entity);
      local_to_world->local_to_world = Matrix4x4::TRS(derived_transform->position, derived_transform->rotation, derived_transform->scale);
    }
  }

}
