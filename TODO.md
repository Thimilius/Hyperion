# TODO

## App
- Multiple windows
- Internal event queue

## Assets
- Textures
    - Cubemaps
    - RenderTextures
- Fonts
- Texture atlas
- Serialization
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
- Rename RenderMeshComponent to MeshComponent
- Simple frustum culling
- Textures
- Framebuffers
- Lighting
    - Storage buffers
	- Move lighting setup into render pipeline
- Control per object data in render pipeline
- Async readback of textures/buffers

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
