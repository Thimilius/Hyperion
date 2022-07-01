# TODO

## App
- Support for multiple windows
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
- Add ability to properly define system execution order
- Archetypes with overwrites

## Physics
- Proper updating of colliders and their transforms
- Rigidbodies

## Rendering
- Lighting
    - Spotlights
    - Frustum culling
  - Use forward clustered shading
- Async readback
    - Textures
    - Buffers
- Proper Vulkan implementation
- Better system to support "globals" (similar to Unity)
    - Setting of global buffers
    - Setting of global shader properties

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
		- Input Sliders
		- Main Thread performance is barely acceptable (it actually really isn't)
- Font rendering
    - SDF

## Editor
- Fix rotation of child entities with gizmo in world space mode
