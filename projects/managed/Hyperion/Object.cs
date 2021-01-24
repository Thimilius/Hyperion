﻿using System.Runtime.CompilerServices;

namespace Hyperion {
    public class Object {
        public string Name {
            get => Binding_GetName(this);
            set => Binding_SetName(this, value);
        }

        public Object() => Binding_ObjectCtor(this, null);
        public Object(string name) => Binding_ObjectCtor(this, name);

        public override string ToString() => Name;

        public static void Destroy(Object obj) => Binding_Destroy(obj);

        public static implicit operator bool(Object obj) => Binding_IsNativeAlive(obj);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Binding_ObjectCtor(Object obj, string name);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern string Binding_GetName(Object obj);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Binding_SetName(Object obj, string name);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Binding_Destroy(Object obj);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern bool Binding_IsNativeAlive(Object obj);
    }
}