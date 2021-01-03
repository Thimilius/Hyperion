using System;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Threading;

namespace HyperionEngine {
    public static class Application {
        public static void Do() {
            Console.WriteLine("Hello from managed!");

            Log("String passed from managed");
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void Log(string message);
    }
}
