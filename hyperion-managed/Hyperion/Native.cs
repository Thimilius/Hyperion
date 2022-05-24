using System;
using System.Diagnostics;
using System.Reflection;
using System.Runtime.InteropServices;

namespace Hyperion {
  internal static class Native {
    [UnmanagedCallersOnly]
    internal static IntPtr GetTypeByName(IntPtr name) {
      string typeName = Marshal.PtrToStringUTF8(name);
      Assembly assembly = Assembly.GetExecutingAssembly();
      Type type = assembly.GetType(typeName);
      if (type == null) {
        Engine.LogError($"Failed to find type by name: {typeName}!");
      }
      return GCHandle.ToIntPtr(GCHandle.Alloc(type));
    }
    
    [UnmanagedCallersOnly]
    internal static IntPtr CreateManagedObject(IntPtr typeHandle, IntPtr nativeHandle) {
      Type type = GCHandle.FromIntPtr(typeHandle).Target as Type;
      
      Object instance = Activator.CreateInstance(type) as Object;
      instance.NativeHandle = nativeHandle;

      return GCHandle.ToIntPtr(GCHandle.Alloc(instance));
    } 
  }
}