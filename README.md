# Hyperion

<div align="center">
    <img src="run_tree/icon/icon.png" alt="Logo" width="512"/>
</div>

## Overview

The main goal of the engine is to serve as a learning tool while developing.
It is not trying to be the next Unreal, Unity, CryEngine, Godot or similar.
Instead it focuses on how the different features of those engines could be implemented and tries to explore the architecture of how these systems have to interact together.

## Notable Features

This is not necessarily a summary of all included features but instead the most notables ones.

- Engine
    - Architecture for supporting multiple platforms (Currently Windows is the only existing implementation)
    - Engine loop abstraction for controlling subsystem update order
- Linear math library
- ECS using sparse sets
    - No limitations for entity/component count
    - Entity/Transformation hierarchy
    - World serilization
- Physic colliders
- Render Engine
    - Pipelined Main and Render Thread
    - High level API to support implementation of different render pipelines (like Unity's scriptable render pipeline)
    - View frustum culling
- Assets
    - Hot Reloading of Shaders
- Scripting with C# using modern .NET 6 runtime (instead of mono)
- User interface
    - Immediate mode gui
    - Retained mode gui
- Editor
    - Play mode in editor
    - Transformation gizmos


## Dependencies

Currently the engine relies on the following libraries:
- assimp
- bullet3
- freetype
- shaderc
- stb
- vulkan-memory-allocator
- yaml-cpp
- rttr

It does however also deliberatly **renounce** the use of the following typically used libraries:
- Window/Context creation (e.g. GLFW or SDL)
- Math (e.g. GLM)
- Entity Management (e.g. entt)
- UI (e.g. Dear ImGui)

This way the gaining of knowledge is retained by actually having to implement those systems and functionalities.

## Building

Currently the engine uses Premake5 as a build system.
The generate.bat script generates the required visual studio solution file for building on Windows using MSVC.
Currently there is no support for other platforms or other compilers.
