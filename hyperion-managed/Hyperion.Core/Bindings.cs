using System;
using System.Runtime.InteropServices;

namespace Hyperion {
  internal static unsafe class Bindings {
    [StructLayout(LayoutKind.Sequential)]
    internal struct AllBindings {
      internal readonly CoreBindings CoreBindings;
      internal readonly LogBindings LogBindings;
      internal readonly WorldManagerBindings WorldManagerBindings;
      internal readonly WorldBindings WorldBindings;
    }
    
    [StructLayout(LayoutKind.Sequential)]
    internal struct CoreBindings {
      internal readonly delegate *unmanaged<string, void> Exception;
    }
    
    [StructLayout(LayoutKind.Sequential)]
    internal struct LogBindings {
      internal readonly delegate *unmanaged<string, void> Trace;
      internal readonly delegate *unmanaged<string, void> Info;
      internal readonly delegate *unmanaged<string, void> Warn;
      internal readonly delegate *unmanaged<string, void> Error;  
    }
    
    [StructLayout(LayoutKind.Sequential)]
    internal struct WorldManagerBindings {
      internal readonly delegate *unmanaged<IntPtr> GetActiveWorld;
    }
    
    [StructLayout(LayoutKind.Sequential)]
    internal struct WorldBindings {
      internal readonly delegate *unmanaged<IntPtr, IntPtr> GetName;
      internal readonly delegate *unmanaged<IntPtr, string, void> SetName;
    }
    
    internal static CoreBindings Core { get; set; }
    internal static LogBindings Log { get; set; }
    internal static WorldManagerBindings WorldManager { get; set; }
    internal static WorldBindings World { get; set; }
  }
}