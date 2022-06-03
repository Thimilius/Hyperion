﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
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
    private static readonly List<IApplication> s_Applications = new();

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
    
    internal static void Bootstrap(IntPtr coreBootstrapArgumentsPointer, IEnumerable<Assembly> assemblies) {
      CoreBootstrapArguments *coreBootstrapArguments = (CoreBootstrapArguments *)coreBootstrapArgumentsPointer;

      Bindings.Core = coreBootstrapArguments->NativeBindings.CoreBindings;
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

      RegisterApplications(assemblies);
    }

    [UnmanagedCallersOnly]
    private static void Initialize() {
      try {
        foreach (IApplication application in s_Applications) {
          application.Initialize();
        }
      } catch (Exception e) {
        Bindings.Core.Exception(e.ToString());
      }
    }
    
    [UnmanagedCallersOnly]
    private static void Update() {
      try {
        foreach (IApplication application in s_Applications) {
          application.Update();
        }
      } catch (Exception e) {
        Bindings.Core.Exception(e.ToString());
      }
    }
    
    [UnmanagedCallersOnly]
    private static void Shutdown() {
      try {
        foreach (IApplication application in s_Applications) {
          application.Shutdown();
        }
      } catch (Exception e) {
        Bindings.Core.Exception(e.ToString());
      }
    }

    private static void RegisterApplications(IEnumerable<Assembly> assemblies) {
      Type @interface = typeof(IApplication);
      IEnumerable<Type> types = assemblies.SelectMany(a => a.GetTypes()).Where(t => !t.IsAbstract && @interface.IsAssignableFrom(t));

      foreach (Type type in types) {
        try {
          IApplication callback = Activator.CreateInstance(type) as IApplication;
          s_Applications.Add(callback);
        } catch (Exception e) {
          Log(e);
        }
      }
    }
  }
}