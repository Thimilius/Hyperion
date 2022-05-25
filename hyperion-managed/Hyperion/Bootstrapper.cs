using System;
using System.Runtime.InteropServices;

namespace Hyperion {
  internal static unsafe class Bootstrapper {
    [StructLayout(LayoutKind.Sequential)]
    private struct ManagedBindings {
      public delegate *unmanaged<void> EngineInitialize;
      public delegate *unmanaged<void> EngineUpdate;
      public delegate *unmanaged<void> EngineShutdown;

      public delegate *unmanaged<IntPtr, IntPtr> GetTypeByName;
      public delegate *unmanaged<IntPtr, IntPtr, IntPtr> CreateManagedObject;
    }
    
    [StructLayout(LayoutKind.Sequential)]
    private struct BootstrapArguments {
      public readonly Bindings.AllBindings NativeBindings;
      public readonly delegate *unmanaged<ManagedBindings *, void> ForwardManagedBindings;
    }

    [UnmanagedCallersOnly]
    private static int Bootstrap(IntPtr arguments) {
      var bootstrapArguments = (BootstrapArguments *)arguments;
      
      Bindings.Log = bootstrapArguments->NativeBindings.LogBindings;
      Bindings.WorldManager = bootstrapArguments->NativeBindings.WorldManagerBindings;
      Bindings.World = bootstrapArguments->NativeBindings.WorldBindings;
      
      var functionPointers = new ManagedBindings {
        EngineInitialize = &Engine.Initialize,
        EngineUpdate = &Engine.Update,
        EngineShutdown = &Engine.Shutdown,
        
        GetTypeByName = &Native.GetTypeByName,
        CreateManagedObject = &Native.CreateManagedObject,
      };
      bootstrapArguments->ForwardManagedBindings(&functionPointers);
      
      return 0;
    }
  }
}