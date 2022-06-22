# Hyperion

<div align="center">
    <img src="run_tree/icon/icon.png" alt="Logo" width="512"/>
</div>

## Overview

The main goal of this project is to serve as a learning tool while developing.
It is not trying to be the next Unreal, Unity, CryEngine, Godot or similar.
Instead it focuses on how all the different features of those engines could be implemented while providing a similar easy to use high-level API.
The focus is on exploring the needed architecture so that these systems can interact together.

## Notable Features

This is not necessarily a summary of all included features but instead the most notables ones.

- Engine
    - Architecture for supporting multiple platforms (Currently Windows is the only existing implementation as it is the main target)
    - Engine loop abstraction for controlling subsystem update order
    - Most subsystems are abstracted as drivers/modules to allow for different implementations
- Custom math library
- Custom ECS using sparse sets
    - No limitations for entity/component count
    - Entity/Transformation hierarchy
    - World serialization
- Render Engine
    - Pipelined Main and Render Thread
    - High level API to support implementation of different render pipelines (like Unity's scriptable render pipeline)
    - Support for multiple graphics API backends
    - View frustum culling
- Assets
    - Hot Reloading of Shaders
- Scripting with C# using modern .NET 6 runtime (instead of mono)
- Custom user interface
    - Immediate mode
    - Retained mode
- Editor
    - Play mode in editor
    - Assembly reloading while engine is running
    - Transformation gizmos

Some of those features should more be seen as a "proof of concept" in their architecture as they are often not in a production ready state.
The best example for that is the C# scripting where the focus is the implementation of the required groundwork (interaction with the engine, object lifetime management, etc.) rather than providing a full set of bindings for everything.  

## Dependencies

For dependency management the project uses the *Conan* package manage.

Currently the engine relies on the following libraries:
- [*assimp*](https://github.com/assimp/assimp) for loading meshes
- [*bullet3*](https://github.com/bulletphysics/bullet3) for providing a physics driver
- [*freetype*](https://github.com/freetype/freetype) for loading fonts
- [*glad*](https://github.com/Dav1dde/glad) for loading OpenGL extensions
- [*nameof*](https://github.com/Neargye/nameof) for simplifying getting the name of types at compile-time
- [*optick*](https://github.com/bombomby/optick) for integration with the frame-debugger
- [*shaderc*](https://github.com/google/shaderc) for compiling glsl to SPIR-V
- [*soloud*](https://github.com/jarikomppa/soloud) for providing an audio driver
- [*stb*](https://github.com/nothings/stb) for loading images
- [*vulkan-memory-allocator*](https://github.com/GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator) for simplifying memory management in Vulkan 
- [*yaml-cpp*](https://github.com/jbeder/yaml-cpp) for serializing data 
- [*rttr*](https://github.com/rttrorg/rttr) for allowing querying of type information at runtime (used by serialization and the editor)

It does however also deliberately renounce the use of the following typically used libraries:
- Window/Context creation (e.g. GLFW or SDL)
- Math (e.g. GLM)
- Entity Management (e.g. entt)
- UI (e.g. Dear ImGui)

This way the gaining of knowledge can be maximized by actually having to implement those systems and functionalities.

## Building

Currently the engine uses *Premake5* as a build system.
The *generate.bat* script generates the required Visual Studio solution file for building on Windows using MSVC and set ups all required packages (Premake5 and Conan have to be on the path).
Currently there is no support for other platforms or other compilers.

Some notable compiler settings include:
* Usage of C++20
* No use of exceptions
* No use of RTTI
* Warnings are treated as errors
