# TODO

## App
- Multiple windows
- Internal event queue
- Time Main Thread and Render Thread individually

## Assets
- Textures
    - Cubemaps
    - RenderTextures
- Fonts
- Texture atlas
- Duplication
- Error shader/material
- Data update for textures/meshes

## Audio
- SoLoud

## ECS
- Hierarchy should use callbacks for adding/removing entities
- Hierarchy callbacks for transform updates
- System execution order
- Entity/Component duplication
- Archetypes with overwrites
- Component pool pointer stability (if necessary)

## Physics
- Proper updating of colliders and their transforms
- Rigidbodies

## Rendering
- Frustum culling for lights
- Textures
- Framebuffers
- Lighting
    - Storage buffers
	- Move lighting setup into render pipeline
- Control per object data in render pipeline
- Async readback of textures/buffers

## Serialization
- Support for arrays and maps
- Include base types in serialization
- Generic YAML serialization
- Assets
- Binary serialization

### Shaders
- Proper shader language
    - Default values for properties
	- Render queue
	- Rasterizer state
	- New (and better/prettier) syntax
- Recompilation/Hot loading

## Scripting
- .NET 5/6 or Mono?

## UI
- Immediate or Retained? (Maybe both?)
- Font rendering (Rasterized or SDF?)
