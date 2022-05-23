using System.Runtime.InteropServices;

namespace Hyperion {
  internal static unsafe class Bindings {
    [StructLayout(LayoutKind.Sequential)]
    internal struct AllBindings {
      internal readonly LogBindings LogBindings;
    }
    
    [StructLayout(LayoutKind.Sequential)]
    internal struct LogBindings {
      internal readonly delegate *unmanaged<string, void> Trace;
      internal readonly delegate *unmanaged<string, void> Info;
      internal readonly delegate *unmanaged<string, void> Warn;
      internal readonly delegate *unmanaged<string, void> Error;  
    }
    
    internal static LogBindings Log { get; set; }
  }
}