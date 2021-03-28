using System;
using System.Collections.Generic;
using System.Linq;

namespace Hyperion {
    public static class Engine {
        private static readonly List<IApplication> s_Applications = new List<IApplication>();

        public static void Log(Object @object) => Log(@object?.ToString());
        public static void Log(object @object) => Log(@object?.ToString());
        public static void Log(string message) => Console.WriteLine(message ?? "null");
        public static void Log(string format, params object[] args) => Console.WriteLine(format ?? "null", args);

        internal static void Register() {
            RegisterApplications();
        }

        internal static void Initialize() {
            foreach (IApplication applicationCallback in s_Applications) {
                applicationCallback.Initialize();
            }
        }

        internal static void Update(float deltaTime) {
            foreach (IApplication applicationCallback in s_Applications) {
                applicationCallback.Update(deltaTime);
            }
        }

        internal static void FixedUpdate(float fixedDeltaTime) {
            foreach (IApplication applicationCallback in s_Applications) {
                applicationCallback.FixedUpdate(fixedDeltaTime);
            }
        }

        internal static void Shutdown() {
            foreach (IApplication applicationCallback in s_Applications) {
                applicationCallback.Shutdown();
            }
        }

        private static void RegisterApplications() {
            Type @interface = typeof(IApplication);
            IEnumerable<Type> types = AppDomain.CurrentDomain.GetAssemblies().SelectMany(a => a.GetTypes()).Where(t => !t.IsAbstract && @interface.IsAssignableFrom(t));

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
