using System.Runtime.InteropServices;

namespace Hyperion {
  public static class Engine {
    public static void Log(object @object) => Log(@object?.ToString());
    public static void Log(string format, params object[] args) => Log(string.Format(format, args));
    public static void Log(string message) => LogTrace(message);
    
    public static unsafe void LogTrace(string message) => Bindings.LogTrace(message); 
    
    [UnmanagedCallersOnly]
    internal static void Initialize() {
      Log(nameof(Initialize));
    }

    [UnmanagedCallersOnly]
    internal static void Update() {
      Log(nameof(Update));
    }
    
    [UnmanagedCallersOnly]
    internal static void Shutdown() {
      Log(nameof(Shutdown));
    }
  }
}