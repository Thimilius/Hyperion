using System;
using System.Collections.Generic;
using System.Reflection;
using System.Runtime.InteropServices;

namespace Hyperion {
  internal static unsafe class Native {
    private static readonly Dictionary<Type, IntPtr> s_TypeHandles = new();

    internal static IntPtr GetTypeHandle<T>() {
      return s_TypeHandles[typeof(T)];
    }
    
    [UnmanagedCallersOnly]
    internal static IntPtr GetTypeByName(IntPtr name) {
      try {
        string typeName = Marshal.PtrToStringUTF8(name);
        Assembly assembly = Assembly.GetExecutingAssembly();
        Type type = assembly.GetType(typeName);
        if (type == null) {
          throw new Exception($"Type with name: '{typeName}' could not be found!");
        }

        IntPtr handle = GCHandle.ToIntPtr(GCHandle.Alloc(type));
        s_TypeHandles[type] = handle;
        
        return handle;
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
        Type type = GCHandle.FromIntPtr(typeHandle).Get<Type>();
      
        // NOTE: Using the Activator may or may not be the fastest route.
        // One alternative would be to create a compiled Expression and call that instead.
        // This does however have a one time setup cost which is only worth it when calling this function A LOT.
        // So for now we just use this easy method and do not bother.
        Object instance = Activator.CreateInstance(type, true) as Object;
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
        
        Object instance = gcHandle.Get<Object>();
        instance.NativeHandle = IntPtr.Zero;
        
        gcHandle.Free();
      } catch (Exception e) {
        Bindings.Core.Exception(e.ToString());
      }
    }
  }
}