using System;
using System.Reflection;
using System.Runtime.InteropServices;

namespace Hyperion {
  internal static unsafe class Native {
    [UnmanagedCallersOnly]
    internal static IntPtr GetTypeByName(IntPtr name) {
      try {
        string typeName = Marshal.PtrToStringUTF8(name);
        Assembly assembly = Assembly.GetExecutingAssembly();
        Type type = assembly.GetType(typeName);
        if (type == null) {
          Engine.LogError($"Failed to find type by name: {typeName}!");
        }

        return GCHandle.ToIntPtr(GCHandle.Alloc(type));
      } catch (Exception e) {
        Bindings.Core.Exception(e.ToString());
        return IntPtr.Zero;
      }
    }
    
    [UnmanagedCallersOnly]
    internal static void DestroyType(IntPtr managedHandle) {
      try {
        GCHandle.FromIntPtr(managedHandle).Free();
      } catch (Exception e) {
        Bindings.Core.Exception(e.ToString());
      }
    }
    
    [UnmanagedCallersOnly]
    internal static IntPtr CreateManagedObject(IntPtr typeHandle, IntPtr nativeHandle) {
      try {
        Type type = GCHandle.FromIntPtr(typeHandle).Target as Type;
      
        Object instance = Activator.CreateInstance(type) as Object;
        instance.NativeHandle = nativeHandle;

        return GCHandle.ToIntPtr(GCHandle.Alloc(instance));
      } catch (Exception e) {
        Bindings.Core.Exception(e.ToString());
        return IntPtr.Zero;
      }
    }

    [UnmanagedCallersOnly]
    internal static void DestroyManagedObject(IntPtr managedHandle) {
      try {
        GCHandle gcHandle = GCHandle.FromIntPtr(managedHandle);
        Object obj = gcHandle.Target as Object;
        
        obj.NativeHandle = IntPtr.Zero;
        
        gcHandle.Free();
      } catch (Exception e) {
        Bindings.Core.Exception(e.ToString());
      }
    }
  }
}