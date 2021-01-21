using System;

namespace Hyperion {
    public static class Application {
        public static void Run() {
            Object obj = new Object("My Object");
            Console.WriteLine(obj.Name);
            Object.Destroy(obj);
        }
    }
}
