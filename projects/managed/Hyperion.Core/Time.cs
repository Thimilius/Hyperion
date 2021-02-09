using System.Runtime.CompilerServices;

namespace Hyperion {
    public static class Time {
        public static float ElapsedTime => Binding_GetElapsedTime();
        public static float DeltaTime => Binding_GetDeltaTime();
        public static float FixedDeltaTime => Binding_GetFixedDeltaTime();

        [MethodImpl(MethodImplOptions.InternalCall)] private static extern float Binding_GetDeltaTime();
        [MethodImpl(MethodImplOptions.InternalCall)] private static extern float Binding_GetElapsedTime();
        [MethodImpl(MethodImplOptions.InternalCall)] private static extern float Binding_GetFixedDeltaTime();
    }
}
