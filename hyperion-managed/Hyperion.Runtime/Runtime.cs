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
    private static void LoadContext(IntPtr coreBootstrapArgumentsPointer) {
      try {
        // We expect all managed assemblies to be next to us.
        string assemblyDirectory = Path.GetDirectoryName(typeof(Runtime).Assembly.Location);
        string coreAssemblyName = "Hyperion.Core.dll";
        string coreAssemblyPath = Path.Combine(assemblyDirectory, coreAssemblyName);
        string sandboxAssemblyName = "Hyperion.Sandbox.dll";
        string sandboxAssemblyPath = Path.Combine(assemblyDirectory, sandboxAssemblyName);
        
        AssemblyLoadContext loadContext = new RuntimeLoadContext(assemblyDirectory);
        s_LoadContext = new WeakReference(loadContext);

        Assembly coreAssembly = loadContext.LoadFromAssemblyPath(coreAssemblyPath);
        loadContext.LoadFromAssemblyPath(sandboxAssemblyPath);
        
        // We need to bootstrap the core assembly (exchange function pointers).
        Type type = coreAssembly.GetType("Hyperion.Engine");
        type.InvokeMember(
          "Bootstrap",
          BindingFlags.NonPublic | BindingFlags.Static | BindingFlags.InvokeMethod,
          null,
          null,
          new object[] {
            coreBootstrapArgumentsPointer,
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