using System;
using System.Runtime.InteropServices;

namespace Hyperion {
  internal static unsafe class Bootstrapper {
    [StructLayout(LayoutKind.Sequential)]
    private struct NativeFunctionPointers {
      public readonly delegate *unmanaged<string, void> LogTrace;
    }
    
    [StructLayout(LayoutKind.Sequential)]
    private struct ManagedFunctionPointers {
      public delegate *unmanaged<void> EngineInitialize;
      public delegate *unmanaged<void> EngineUpdate;
      public delegate *unmanaged<void> EngineShutdown;
    }
    
    [StructLayout(LayoutKind.Sequential)]
    private struct BootstrapArguments {
      public readonly NativeFunctionPointers NativeFunctionPointers;
      public readonly delegate *unmanaged<ManagedFunctionPointers *, void> ForwardPointersCallback;
    }

    internal static int Bootstrap(IntPtr arguments, int argumentSize) {
      if (argumentSize < Marshal.SizeOf<BootstrapArguments>()) {
        return 1;
      }

      var bootstrapArguments = Marshal.PtrToStructure<BootstrapArguments>(arguments);

      Bindings.LogTrace = bootstrapArguments.NativeFunctionPointers.LogTrace;
      
      var functionPointers = new ManagedFunctionPointers {
        EngineInitialize = &Engine.Initialize,
        EngineUpdate = &Engine.Update,
        EngineShutdown = &Engine.Shutdown
      };

      bootstrapArguments.ForwardPointersCallback(&functionPointers);
      
      return 0;
    }
  }
}