# TODO

## App
- Multiple windows
- Internal event queue
- Time Main Thread and Render Thread individually

## Assets
- Asset registry
- Textures
	- Cubemaps
- Duplication
- Data update for textures

## Audio
- SoLoud implementation

## ECS
- Hierarchy should use callbacks when adding/removing HierarchyComponent
- Hierarchy callbacks for transform updates
- Entity instantiation should include hierarchy
- System execution order
- Archetypes with overwrites

## Physics
- Proper updating of colliders and their transforms
- Rigidbodies

## Rendering
- Proper Vulkan implementation
- Lighting
	- Spotlights
  - Storage buffers
	- Frustum culling
- Async readback
	- Textures
	- Buffers

## Serialization
- Support for arrays and maps
- Generic YAML serialization
- Assets
- Binary serialization

### Shaders
- Proper shader language
  - Default values for properties
	- New (and better/prettier) syntax

## Scripting
- Add more bindings

## UI
- Immediate
	- Fix layout
- Font rendering
    - SDF

## Editor
- Fix rotation of child entities with gizmo using WorldToLocal helpers
