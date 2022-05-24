using System;
using System.Runtime.InteropServices;

namespace Hyperion {
  internal static unsafe class Bootstrapper {
    [StructLayout(LayoutKind.Sequential)]
    private struct ManagedBindings {
      public delegate *unmanaged<void> EngineInitialize;
      public delegate *unmanaged<void> EngineUpdate;
      public delegate *unmanaged<void> EngineShutdown;
      
      public delegate *unmanaged<IntPtr, IntPtr> CreateManagedWorld;
    }
    
    [StructLayout(LayoutKind.Sequential)]
    private struct BootstrapArguments {
      public readonly Bindings.AllBindings NativeBindings;
      public readonly delegate *unmanaged<ManagedBindings *, void> ForwardManagedBindings;
    }

    internal static int Bootstrap(IntPtr arguments, int argumentSize) {
      if (argumentSize < Marshal.SizeOf<BootstrapArguments>()) {
        return 1;
      }

      var bootstrapArguments = Marshal.PtrToStructure<BootstrapArguments>(arguments);
      
      Bindings.Log = bootstrapArguments.NativeBindings.LogBindings;
      Bindings.WorldManager = bootstrapArguments.NativeBindings.WorldManagerBindings;
      Bindings.World = bootstrapArguments.NativeBindings.WorldBindings;
      
      var functionPointers = new ManagedBindings {
        EngineInitialize = &Engine.Initialize,
        EngineUpdate = &Engine.Update,
        EngineShutdown = &Engine.Shutdown,
        
        CreateManagedWorld = &World.CreateManagedWorld,
      };
      bootstrapArguments.ForwardManagedBindings(&functionPointers);
      
      return 0;
    }
  }
}