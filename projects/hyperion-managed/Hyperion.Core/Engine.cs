using System;
using System.Collections.Generic;
using System.Linq;

namespace Hyperion {
    public static class Engine {
        private static readonly List<IApplication> s_Applications = new List<IApplication>();
        private static readonly List<ILogger> s_Loggers = new List<ILogger>();

        public static void Log(Object @object) => Log(@object?.ToString());
        public static void Log(object @object) => Log(@object?.ToString());
        public static void Log(string format, params object[] args) => Log(string.Format(format, args));
        public static void Log(string message) => LogTrace(message);

        public static void LogTrace(string message) {
            foreach (ILogger logger in s_Loggers) {
                logger.Trace(message);
            }
        }
        public static void LogInfo(string message) {
            foreach (ILogger logger in s_Loggers) {
                logger.Info(message);
            }
        }
        public static void LogWarn(string message) {
            foreach (ILogger logger in s_Loggers) {
                logger.Info(message);
            }
        }
        public static void LogError(string message) {
            foreach (ILogger logger in s_Loggers) {
                logger.Info(message);
            }
        }

        internal static void Register() {
            s_Loggers.Add(new EngineLogger());
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
