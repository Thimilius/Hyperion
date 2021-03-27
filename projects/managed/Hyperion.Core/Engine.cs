using System;
using System.Collections.Generic;
using System.Linq;

namespace Hyperion {
    public static class Engine {
        private static readonly List<IApplication> s_ApplicationCallbacks = new List<IApplication>();

        public static void Log(Object @object) => Log(@object?.ToString());
        public static void Log(object @object) => Log(@object?.ToString());
        public static void Log(string message) => Console.WriteLine(message ?? "null");
        public static void Log(string format, params object[] args) => Console.WriteLine(format ?? "null", args);

        internal static void Initialize() {
            InitializeApplications();

            foreach (IApplication applicationCallback in s_ApplicationCallbacks) {
                applicationCallback.Initialize();
            }
        }

        internal static void Update(float deltaTime) {
            foreach (IApplication applicationCallback in s_ApplicationCallbacks) {
                applicationCallback.Update(deltaTime);
            }
        }

        internal static void FixedUpdate(float fixedDeltaTime) {
            foreach (IApplication applicationCallback in s_ApplicationCallbacks) {
                applicationCallback.FixedUpdate(fixedDeltaTime);
            }
        }

        internal static void Shutdown() {
            foreach (IApplication applicationCallback in s_ApplicationCallbacks) {
                applicationCallback.Shutdown();
            }
        }

        private static void InitializeApplications() {
            Type @interface = typeof(IApplication);
            IEnumerable<Type> types = AppDomain.CurrentDomain.GetAssemblies().SelectMany(a => a.GetTypes()).Where(t => !t.IsAbstract && @interface.IsAssignableFrom(t));

            foreach (Type type in types) {
                try {
                    IApplication callback = Activator.CreateInstance(type) as IApplication;
                    s_ApplicationCallbacks.Add(callback);
                } catch (Exception e) {
                    Log(e);
                }
            }
        }
    }
}
