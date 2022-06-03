using System;
using System.Reflection;
using System.Runtime.Loader;

namespace Hyperion {
  internal class ManagedLoadContext : AssemblyLoadContext {
    private readonly AssemblyDependencyResolver m_Resolver;

    public ManagedLoadContext(string pluginPath) : base(isCollectible: true) {
      m_Resolver = new AssemblyDependencyResolver(pluginPath);
    }

    protected override Assembly Load(AssemblyName assemblyName) {
      string assemblyPath = m_Resolver.ResolveAssemblyToPath(assemblyName);
      return assemblyPath == null ? null : LoadFromAssemblyPath(assemblyPath);
    }

    protected override IntPtr LoadUnmanagedDll(string unmanagedDllName) {
      string libraryPath = m_Resolver.ResolveUnmanagedDllToPath(unmanagedDllName);
      return libraryPath == null ? IntPtr.Zero : LoadUnmanagedDllFromPath(libraryPath);
    }
  }
}