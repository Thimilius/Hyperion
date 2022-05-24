using System;
using System.Runtime.InteropServices;

namespace Hyperion {
  public class Object {
    internal IntPtr NativeHandle;

    protected Object() {
            
    }
    
    internal static IntPtr CreateManagedObject<T>(IntPtr nativeHandle) where T : Object, new() {
      return GCHandle.ToIntPtr(GCHandle.Alloc(new T {
        NativeHandle = nativeHandle
      }));
    }
  }
}