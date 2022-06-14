using System;
using System.IO;
using System.Reflection;
using System.Runtime.InteropServices;
using System.Runtime.Loader;

namespace Hyperion {
  [StructLayout(LayoutKind.Sequential)]
  internal unsafe struct RuntimeNativeBindings {
    public readonly delegate *unmanaged<string, void> Exception;
  }
  
  [StructLayout(LayoutKind.Sequential)]
  internal unsafe struct RuntimeManagedBindings {
    public delegate *unmanaged<IntPtr, void> LoadContext;
    public delegate *unmanaged<void> UnloadContext;
  }
  
  [StructLayout(LayoutKind.Sequential)]
  internal unsafe struct RuntimeBootstrapArguments {
    public readonly RuntimeNativeBindings NativeBindings;
    public readonly delegate *unmanaged<RuntimeManagedBindings *, void> ManagedBindingsCallback;
  }

  [StructLayout(LayoutKind.Sequential)]
  internal struct LoadContextArguments {
    public readonly IntPtr CoreAssemblyName;
    public readonly IntPtr AssemblyName;
      
    public readonly IntPtr CoreBootstrapArguments;
  }

  internal static unsafe class Runtime {
    private static RuntimeNativeBindings s_RuntimeNativeBindings;
    private static WeakReference s_LoadContext;
    
    [UnmanagedCallersOnly]
    private static int Bootstrap(IntPtr runtimeArgumentsPointer) {
      try {
        RuntimeBootstrapArguments *runtimeBootstrapArguments = (RuntimeBootstrapArguments *)runtimeArgumentsPointer;

        s_RuntimeNativeBindings = runtimeBootstrapArguments->NativeBindings;
        
        RuntimeManagedBindings runtimeManagedBindings = new RuntimeManagedBindings {
          LoadContext = &LoadContext,
          UnloadContext = &UnloadContext,
        };
        runtimeBootstrapArguments->ManagedBindingsCallback(&runtimeManagedBindings);
      } catch (Exception e) {
        s_RuntimeNativeBindings.Exception("Failed to bootstrap runtime!\n" + e);
      }

      return 0;
    }

    [UnmanagedCallersOnly]
    private static void LoadContext(IntPtr loadContextArgumentsPointer) {
      try {
        LoadContextArguments* loadContextArguments = (LoadContextArguments *)loadContextArgumentsPointer;
        
        // We expect all managed assemblies to be next to us.
        string assemblyDirectory = Path.GetDirectoryName(typeof(Runtime).Assembly.Location)!;
        
        AssemblyLoadContext loadContext = new RuntimeLoadContext(assemblyDirectory);
        s_LoadContext = new WeakReference(loadContext);

        string coreAssemblyName = Marshal.PtrToStringUTF8(loadContextArguments->CoreAssemblyName)!;
        string coreAssemblyPath = Path.Combine(assemblyDirectory, coreAssemblyName);
        Assembly coreAssembly = loadContext.LoadFromAssemblyPath(coreAssemblyPath);
        
        string sandboxAssemblyName = Marshal.PtrToStringUTF8(loadContextArguments->AssemblyName);
        if (!string.IsNullOrEmpty(sandboxAssemblyName)) {
          string sandboxAssemblyPath = Path.Combine(assemblyDirectory, sandboxAssemblyName);
          if (File.Exists(sandboxAssemblyPath)) {
            loadContext.LoadFromAssemblyPath(sandboxAssemblyPath);  
          }
        }
        
        // We need to bootstrap the core assembly (exchange function pointers).
        Type type = coreAssembly.GetType("Hyperion.Engine")!;
        type.InvokeMember(
          "Bootstrap",
          BindingFlags.NonPublic | BindingFlags.Static | BindingFlags.InvokeMethod,
          null,
          null,
          new object[] {
            loadContextArguments->CoreBootstrapArguments,
            loadContext.Assemblies
          }
        );
      } catch (Exception e) {
        s_RuntimeNativeBindings.Exception("Failed to load context!\n" + e);
      }
    }
    
    [UnmanagedCallersOnly]
    private static void UnloadContext() {
      try {
        AssemblyLoadContext loadContext = s_LoadContext.Target as AssemblyLoadContext;
        loadContext?.Unload();

        for (int i = 0; s_LoadContext.IsAlive && (i < 10); i++) {
          GC.Collect();
          GC.WaitForPendingFinalizers();
        }
      } catch (Exception e) {
        s_RuntimeNativeBindings.Exception("Failed to unload context!\n" + e);
      }
    }
  }
}