using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;

namespace Hyperion {
  public static unsafe class Engine {
    [StructLayout(LayoutKind.Sequential)]
    private struct CoreManagedBindings {
      public delegate *unmanaged<void> EngineInitialize;
      public delegate *unmanaged<void> EngineUpdate;
      public delegate *unmanaged<void> EngineShutdown;

      public delegate *unmanaged<IntPtr, IntPtr> GetTypeByName;
      public delegate *unmanaged<IntPtr, IntPtr, IntPtr> CreateManagedObject;
    }
    
    [StructLayout(LayoutKind.Sequential)]
    private struct CoreBootstrapArguments {
      public readonly Bindings.AllBindings NativeBindings;
      public readonly delegate *unmanaged<CoreManagedBindings *, void> ManagedBindingsCallback;
    }

    
    private static readonly List<ILogger> s_Loggers = new();

    public static void Log(object @object) => Log(@object?.ToString());
    public static void Log(string format, params object[] args) => Log(string.Format(format, args));
    public static void Log(string message) => LogTrace(message);

    public static void LogTrace(string message) {
      foreach (var logger in s_Loggers) {
        logger.Trace(message);
      }
    }
    public static void LogInfo(string message) {
      foreach (var logger in s_Loggers) {
        logger.Info(message);
      }
    }
    public static void LogWarn(string message) {
      foreach (var logger in s_Loggers) {
        logger.Warn(message);
      }
    }
    public static void LogError(string message) {
      foreach (var logger in s_Loggers) {
        logger.Error(message);
      }
    }
    
    [UnmanagedCallersOnly]
    internal static void Initialize() {
      try {
        Log("INITIALIZE");
      } catch (Exception e) {
        
      }
    }
    
    [UnmanagedCallersOnly]
    internal static void Update() {
      try {
        Log("Update");
      } catch (Exception e) {
        
      }
    }
    
    [UnmanagedCallersOnly]
    internal static void Shutdown() {
      try {
        Log("Shutdown");
      } catch (Exception e) {
        
      }
    }

    internal static void Bootstrap(IntPtr coreBootstrapArgumentsPointer) {
      CoreBootstrapArguments *coreBootstrapArguments = (CoreBootstrapArguments *)coreBootstrapArgumentsPointer;

      Bindings.Log = coreBootstrapArguments->NativeBindings.LogBindings;
      Bindings.World = coreBootstrapArguments->NativeBindings.WorldBindings;
      Bindings.WorldManager = coreBootstrapArguments->NativeBindings.WorldManagerBindings;
     
      CoreManagedBindings coreManagedBindings = new CoreManagedBindings {
        EngineInitialize = &Initialize,
        EngineUpdate = &Update,
        EngineShutdown = &Shutdown,
        
        GetTypeByName = &Native.GetTypeByName,
        CreateManagedObject = &Native.CreateManagedObject,
      };
      coreBootstrapArguments->ManagedBindingsCallback(&coreManagedBindings);
      
      s_Loggers.Add(new EngineLogger());
    }
  }
}