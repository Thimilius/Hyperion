using System;
using System.Collections.Generic;
using System.IO;
using System.Reflection;
using System.Runtime.InteropServices;
using System.Runtime.Loader;

namespace Hyperion {
  public static class Engine {
    private static readonly List<ILogger> s_Loggers = new();
    private static WeakReference s_LoadContext;

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
        // We expect all managed assemblies to be next to us.
        string assemblyDirectory = Path.GetDirectoryName(typeof(Engine).Assembly.Location);
        string assemblyToLoad = "Hyperion.Sandbox.dll";
        string assemblyPath = Path.Combine(assemblyDirectory, assemblyToLoad);

        AssemblyLoadContext loadContext = new ManagedLoadContext(assemblyDirectory);

        // We need an absolute path to load an assembly.
        Assembly assembly = loadContext.LoadFromAssemblyPath(assemblyPath);
        Log($"Loaded assembly: {assembly}");

        s_LoadContext = new WeakReference(loadContext);
      } catch (Exception e) {
        Log(e);
      }
    }
    
    [UnmanagedCallersOnly]
    internal static void Update() {
    }
    
    [UnmanagedCallersOnly]
    internal static void Shutdown() {
      try {
        AssemblyLoadContext loadContext = s_LoadContext.Target as AssemblyLoadContext;
        loadContext?.Unload();

        for (int i = 0; s_LoadContext.IsAlive && (i < 10); i++) {
          GC.Collect();
          GC.WaitForPendingFinalizers();
        }

        Log("Unloaded assemblies");
      } catch (Exception e) {
        Log(e);
      }
    }

    internal static void Bootstrap() {
      s_Loggers.Add(new EngineLogger());
    }
  }
}