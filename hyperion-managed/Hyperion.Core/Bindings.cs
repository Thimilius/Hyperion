using System;
using System.Runtime.InteropServices;
using Hyperion.Ecs;

namespace Hyperion {
  internal static unsafe class Bindings {
    [StructLayout(LayoutKind.Sequential)]
    internal struct AllBindings {
      internal readonly CoreBindings CoreBindings;
      internal readonly LogBindings LogBindings;
      internal readonly TimeBindings TimeBindings;
      internal readonly InputBindings InputBindings;
      internal readonly WorldManagerBindings WorldManagerBindings;
      internal readonly WorldBindings WorldBindings;
      internal readonly EntityManagerBindings EntityManagerBindings;
      internal readonly NameComponentBindings NameComponentBindings;
    }
    
    [StructLayout(LayoutKind.Sequential)]
    internal struct CoreBindings {
      internal readonly delegate *unmanaged<string, void> Exception;
    }
    
    [StructLayout(LayoutKind.Sequential)]
    internal struct LogBindings {
      internal readonly delegate *unmanaged<string, void> Trace;
      internal readonly delegate *unmanaged<string, void> Info;
      internal readonly delegate *unmanaged<string, void> Warn;
      internal readonly delegate *unmanaged<string, void> Error;  
    }
    
    [StructLayout(LayoutKind.Sequential)]
    internal struct TimeBindings {
      internal readonly delegate *unmanaged<float> GetTime;
      internal readonly delegate *unmanaged<float> GetDeltaTime;
    }
    
    [StructLayout(LayoutKind.Sequential)]
    internal struct InputBindings {
      internal readonly delegate *unmanaged<KeyCode, bool> IsKeyDown;
      internal readonly delegate *unmanaged<KeyCode, bool> IsKeyHold;
      internal readonly delegate *unmanaged<KeyCode, bool> IsKeyUp;
    }
    
    [StructLayout(LayoutKind.Sequential)]
    internal struct WorldManagerBindings {
      internal readonly delegate *unmanaged<IntPtr> GetActiveWorld;
    }
    
    [StructLayout(LayoutKind.Sequential)]
    internal struct WorldBindings {
      internal readonly delegate *unmanaged<IntPtr, IntPtr> GetName;
      internal readonly delegate *unmanaged<IntPtr, string, void> SetName;
      
      internal readonly delegate *unmanaged<IntPtr, IntPtr> GetEntityManager;
    }
    
    [StructLayout(LayoutKind.Sequential)]
    internal struct EntityManagerBindings {
      internal readonly delegate *unmanaged<IntPtr, int> GetEntityCount;

      internal readonly delegate *unmanaged<IntPtr, out EntityId, void> CreateEntity;
      internal readonly delegate *unmanaged<IntPtr, EntityId, void> DestroyEntity;

      internal readonly delegate *unmanaged<IntPtr, IntPtr, EntityId, bool> HasComponent;
      internal readonly delegate *unmanaged<IntPtr, IntPtr, EntityId, void> RemoveComponent;
    }
    
    [StructLayout(LayoutKind.Sequential)]
    internal struct NameComponentBindings {
      internal readonly delegate *unmanaged<IntPtr, EntityId, IntPtr> GetName;
      internal readonly delegate *unmanaged<IntPtr, EntityId, string, void> SetName;
    }
    
    internal static CoreBindings Core { get; set; }
    internal static LogBindings Log { get; set; }
    internal static TimeBindings Time { get; set; }
    internal static InputBindings Input { get; set; }
    internal static WorldManagerBindings WorldManager { get; set; }
    internal static WorldBindings World { get; set; }
    internal static EntityManagerBindings EntityManager { get; set; }
    internal static NameComponentBindings NameComponent { get; set; }
  }
}