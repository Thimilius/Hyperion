//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/math/vector3.hpp"
#include "hyperion/core/math/quaternion.hpp"
#include "hyperion/ecs/component/components/transform_components.hpp"
#include "hyperion/ecs/world/world.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  class TransformUtilities final {
  public:
    inline static Vector3 GetRight(DerivedTransformComponent *derived_transform) { return derived_transform->rotation * Vector3::Right(); }
    inline static Vector3 GetUp(DerivedTransformComponent *derived_transform) { return derived_transform->rotation * Vector3::Up(); }
    inline static Vector3 GetForward(DerivedTransformComponent *derived_transform) { return derived_transform->rotation * Vector3::Forward(); }

    inline static void SetPosition(World *world, EntityId entity, const Vector3 &position) {
      HierarchyComponent *hierarchy = world->GetComponent<HierarchyComponent>(entity);
      LocalTransformComponent *local_transform = world->GetComponent<LocalTransformComponent>(entity);
      if (hierarchy->parent == Entity::EMPTY) {
        local_transform->position = position;
      } else {
        DerivedTransformComponent *parent_derived_transform = world->GetComponent<DerivedTransformComponent>(hierarchy->parent);
        local_transform->position = (parent_derived_transform->rotation.Inverted() * (position - parent_derived_transform->position)) / parent_derived_transform
          ->scale;
      }
      world->GetHierarchy()->UpdateTransform(WorldHierarchyTransformUpdate::Branch, entity);
    }

    inline static void SetRotation(World *world, EntityId entity, const Quaternion &rotation) {
      HierarchyComponent *hierarchy = world->GetComponent<HierarchyComponent>(entity);
      LocalTransformComponent *local_transform = world->GetComponent<LocalTransformComponent>(entity);
      if (hierarchy->parent == Entity::EMPTY) {
        local_transform->rotation = rotation;
      } else {
        DerivedTransformComponent *parent_derived_transform = world->GetComponent<DerivedTransformComponent>(hierarchy->parent);
        local_transform->rotation = parent_derived_transform->rotation * local_transform->rotation;
      }
      world->GetHierarchy()->UpdateTransform(WorldHierarchyTransformUpdate::Branch, entity);
    }

    inline static Vector3 WorldToLocalPosition(World *world, EntityId entity, const Vector3 &position) {
      HierarchyComponent *hierarchy = world->GetComponent<HierarchyComponent>(entity);
      if (hierarchy->parent == Entity::EMPTY) {
        return position;
      } else {
        DerivedTransformComponent *parent_derived_transform = world->GetComponent<DerivedTransformComponent>(hierarchy->parent);
        return (parent_derived_transform->rotation.Inverted() * (position - parent_derived_transform->position)) / parent_derived_transform->scale;
      }
    }

    inline static Vector3 LocalToWorldPosition(World *world, EntityId entity, const Vector3 &position) {
      HierarchyComponent *hierarchy = world->GetComponent<HierarchyComponent>(entity);
      if (hierarchy->parent == Entity::EMPTY) {
        return position;
      } else {
        DerivedTransformComponent *parent_derived_transform = world->GetComponent<DerivedTransformComponent>(hierarchy->parent);
        return (parent_derived_transform->rotation * (parent_derived_transform->scale * position)) + parent_derived_transform->position;
      }
    }

    inline static Quaternion WorldToLocalRotation(World *world, EntityId entity, const Quaternion &rotation) {
      HierarchyComponent *hierarchy = world->GetComponent<HierarchyComponent>(entity);
      if (hierarchy->parent == Entity::EMPTY) {
        return rotation;
      } else {
        LocalTransformComponent *local_transform = world->GetComponent<LocalTransformComponent>(entity);
        DerivedTransformComponent *parent_derived_transform = world->GetComponent<DerivedTransformComponent>(hierarchy->parent);
        return parent_derived_transform->rotation * local_transform->rotation;
      }
    }

    inline static Quaternion LocalToWorldRotation(World *world, EntityId entity, const Quaternion &rotation) {
      HierarchyComponent *hierarchy = world->GetComponent<HierarchyComponent>(entity);
      if (hierarchy->parent == Entity::EMPTY) {
        return rotation;
      } else {
        LocalTransformComponent *local_transform = world->GetComponent<LocalTransformComponent>(entity);
        DerivedTransformComponent *parent_derived_transform = world->GetComponent<DerivedTransformComponent>(hierarchy->parent);
        return parent_derived_transform->rotation * local_transform->rotation;
      }
    }

  private:
    TransformUtilities() = delete;
    ~TransformUtilities() = delete;
  };

}
