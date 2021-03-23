﻿using System;
using System.Linq;
using System.Reflection;

namespace Hyperion {
    public static class Engine {
        public static void Log(Object @object) => Log(@object?.ToString());
        public static void Log(object @object) => Log(@object?.ToString());
        public static void Log(string message) => Console.WriteLine(message ?? "null");
        public static void Log(string format, params object[] args) => Console.WriteLine(format ?? "null", args);

        internal static Type[] ScanAssemblyForImplementedInterface(Assembly assembly, Type @interface) {
            return assembly.GetTypes().Where(type => !type.IsAbstract && type.GetInterfaces().Contains(@interface)).ToArray();
        }
    }
}
