using System.Runtime.CompilerServices;

namespace Hyperion {
    internal class EngineLogger : ILogger {
        public void Info(string message) => Binding_Info(message);
        public void Trace(string message) => Binding_Trace(message);
        public void Warn(string message) => Binding_Warn(message);
        public void Error(string message) => Binding_Error(message);

        [MethodImpl(MethodImplOptions.InternalCall)] private static extern void Binding_Info(string message);
        [MethodImpl(MethodImplOptions.InternalCall)] private static extern void Binding_Trace(string message);
        [MethodImpl(MethodImplOptions.InternalCall)] private static extern void Binding_Warn(string message);
        [MethodImpl(MethodImplOptions.InternalCall)] private static extern void Binding_Error(string message);
    }
}
