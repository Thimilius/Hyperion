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
    void HierarchyTransformSystem::Run(World *world) {
        HYP_PROFILE_SCOPE("HierarchyTransformSystem.Run");

        EntityId root = world->GetHierarchy()->GetFirstRoot();
        uint64 root_count = world->GetHierarchy()->GetRootCount();
        for (uint64 i = 0; i < root_count; i++) {
            HierarchyComponent *root_hierarchy = world->GetComponent<HierarchyComponent>(root);
            bool8 is_not_disabled = world->GetComponent<DisabledComponent>(root) == nullptr;
            bool8 is_not_static = world->GetComponent<StaticComponent>(root) == nullptr;
            if (is_not_disabled && is_not_static) {
                UpdateBranch(world, root, world->GetComponent<HierarchyComponent>(root), nullptr);
            }
            root = root_hierarchy->next_sibling;
        }
    }

    //--------------------------------------------------------------
    void HierarchyTransformSystem::UpdateBranch(World *world, EntityId branch, HierarchyComponent *branch_hierarchy, DerivedTransformComponent *parent_derived_transform) {
        HYP_PROFILE_SCOPE("HierarchyTransformSystem.UpdateBranch");

        LocalTransformComponent *local_transform = world->GetComponent<LocalTransformComponent>(branch);
        DerivedTransformComponent *derived_transform = world->GetComponent<DerivedTransformComponent>(branch);

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
            HierarchyComponent *child_hierarchy = world->GetComponent<HierarchyComponent>(child);
            bool8 is_not_disabled = world->GetComponent<DisabledComponent>(child) == nullptr;
            bool8 is_not_static = world->GetComponent<StaticComponent>(child) == nullptr;
            if (is_not_disabled && is_not_static) {
                UpdateBranch(world, child, child_hierarchy, derived_transform);
            }
            child = child_hierarchy->next_sibling;
        }
    }

    //--------------------------------------------------------------
    void LocalToWorldSystem::Run(World *world) {
        HYP_PROFILE_SCOPE("LocalToWorldSystem.Run");

        auto view = world->GetView<DerivedTransformComponent, LocalToWorldComponent>(ExcludeComponents<DisabledComponent, StaticComponent>());
        for (EntityId entity : view) {
            DerivedTransformComponent *derived_transform = world->GetComponent<DerivedTransformComponent>(entity);
            LocalToWorldComponent *local_to_world = world->GetComponent<LocalToWorldComponent>(entity);
            local_to_world->local_to_world = Matrix4x4::TRS(derived_transform->position, derived_transform->rotation, derived_transform->scale);
        }
    }

}