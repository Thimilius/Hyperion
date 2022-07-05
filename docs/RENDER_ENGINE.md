# Render Engine

## Introduction
This is the documentation and overview of how the rendering (should/will) work.
First of all Hyperion is not an attempt to be the next Unreal Engine, Cry Engine, Unity, ... (you name it).
That means that performance is probably not the highest it could be but we still try to make it fairly efficient.
Mostly however we want to provide a good high level rendering abstraction layer that is customizable.
It should allow one to have different kinds of rendering pipelines (e.g. forward/deferred) if required.

## Pipelined Multithreading
Like most engines Hyperion uses a _Main_ and a _Render_ Thread that are synchronized in a pipelined fashion.
The Main Thread does input, simulation, etc. of the current frame while the Render Thread renders the last frame.
That means the Render Thread is always rendering one frame behind the Main Thread.
The Main Thread extracts the necessary rendering data from the ECS and generates high level rendering commands.
When a frame ends on the Main Thread there is a synchronization point where the data is exchanged.
The Render Thread takes the exchanged rendering commands and translates them to commands for the underlying graphics API.
An ideal timeline of the pipelined frame processing should look like this:
```
               ┌─────────────┐┌─────────────┐┌─────────────┐
Main Thread:   │   Frame N   ││ Frame N + 1 ││ Frame N + 2 │
               └─────────────┘└─────────────┘└─────────────┘
               ┌─────────────┐┌─────────────┐┌─────────────┐
Render Thread: │ Frame N - 1 ││   Frame N   ││ Frame N + 1 │
               └─────────────┘└─────────────┘└─────────────┘
```
Obviously in a lot of scenarios the work between the two threads is not evenly divided.
That means most of the time either the Main Thread has to wait for the Render Thread or the other way ar
There is also an option to turn off the pipelined multithreading and have everything done on the Main Thread.
The data extraction and command translation stays the same regardless, it's just on a single thread.
Currently we do not employ any kind of worker threads on the Main or Render Thread.
That would of course help with performance for some workloads... but the first paragraph still stands.

## Concepts

### Render Asset Context
The **RenderAssetContext** contains every asset that needs to be loaded/unloaded on the GPU.
That includes meshes, shaders, materials, textures, ...
The data for shaders and materials is always copied which is usually fine as it is quite lightweight.
The bigger data from meshes and textures is ONLY copied when the asset is set to _AssetDataAccess::ReadAndWrite_.
In the case where the asset is set to _AssetDataAccess::None_ or _AssetDataAccess::Write_, the data can simply be moved and n

### Render Object Context
The **RenderObjectContext** is the interaction point between the ECS and the rendering engine.
Render systems take the different components and extract every object, camera, light, etc. and copies it into the context
It can be thought of as the snapshot of the simulated world with all the data required to render it.

### Render Pipeline Context
The **RenderPipelineContext** acts as a command buffer to schedule very high level rendering commands.
This is mainly inspired by the Scriptable Render Pipeline from Unity.
The commands include among other things:
  * Culling
  * Drawing meshes
  * Drawing shadows
  * Drawing ui
  * Executing a command buffer

The execution of a command buffer allows pipelines to control the render state and render target.
A _RenderCommandBuffer_ is responsible for holding those more lower level commands.
Those include among others:
  * Clearing/Setting/Blitting of render targets
  * Setting global shader properties and buffers
  * Requesting async readback
  
### Render Frame
The **RenderFrame** is the high level structure that gets exchanged between the Main and Render Thread.
The RenderFrame owns the _RenderAssetContext_, _RenderObjectContext_ and _RenderPipelineContext_.
There are always two static RenderFrames allocated.
One used by the Main Thread to fill it with data and the other one used by the Render Thread to read from it.
It acts as a double buffer so that the synchronization point between the two threads is therefore a simple pointer swap.

### Render Pipeline
The **RenderPipeline** is the highest level in the rendering API stack.
A RenderPipeline lives on the Main Thread and gets passed the RenderPipelineContext of the corresponding RenderFrame for
It uses the RenderPipelineContext to schedule the high level rendering commands which will produce the final render outpu
That way different kinds of rendering pipelines, like forward or deferred, can easily be implemented.

### Render Driver
The **RenderDriver** is the lowest point in the rendering API stack.
A RenderDriver lives on the Render Thread and gets passed the corresponding RenderFrame for reading.
For every underlying graphics API there exists a RenderDriver whose job is twofold:
  1. Loading/Unloading of GPU assets taken from RenderAssetContext
  2. Translation of RenderPipelineContext commands referencing objects in the RenderObjectContext to graphics API command

There was a plan to have a full grown graphics API abstraction layer with implementations for every graphics API.
However that would introduce yet another API level and might also be very time consuming to develop if we want something
By having just the RenderDriver we can act close to the engine and its required rendering features.
That keeps the actual implementation of the graphics API and all its specifics local and enclosed.
This is mainly inspired by Godot which also employs that exact concept.

### Render Context
The **RenderContext** represents the graphics context of the underlying API.
It's mainly responsible for initializing the graphics API, loading extensions and presenting the final render output.
The RenderContext gets implemented by every graphics API and additionally on a lower level by the underlying platform.
