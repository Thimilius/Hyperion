using System;

namespace Hyperion {
    public static class Engine {
        public static void Log(Object @object) => Log(@object.ToString());
        public static void Log(object @object) => Log(@object.ToString());
        public static void Log(string message) => Console.WriteLine(message);
    }
}
