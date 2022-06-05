using System.Runtime.InteropServices;

namespace Hyperion {
  internal static class GCHandleExtensions {
    public static T Get<T>(this GCHandle handle) where T : class {
      T instance = handle.Target as T;
      return instance;
    }
  }
}