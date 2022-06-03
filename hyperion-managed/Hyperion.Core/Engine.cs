using System;
using System.Collections.Generic;
using System.IO;
using System.Reflection;
using System.Runtime.InteropServices;
using System.Runtime.Loader;

namespace Hyperion {
  public static class Engine {
    private static readonly List<ILogger> s_Loggers = new();
    private static AssemblyLoadContext m_LoadContext;

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
      // TODO: Reload assemblies.
    }
    
    [UnmanagedCallersOnly]
    internal static void Update() {
      
    }
    
    [UnmanagedCallersOnly]
    internal static void Shutdown() {
      // TODO: Unload assemblies.
    }

    internal static void Bootstrap() {
      s_Loggers.Add(new EngineLogger());
      
      // We expect all managed assemblies to be next to us.
      string managedAssemblyPath = Path.GetDirectoryName(typeof(Engine).Assembly.Location);
      
      m_LoadContext = new ManagedLoadContext(managedAssemblyPath);
    }
  }
}