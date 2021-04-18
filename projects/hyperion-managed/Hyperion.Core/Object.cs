using System.Runtime.CompilerServices;

namespace Hyperion {
    public abstract class Object {
        public string Name {
            get => Binding_GetName(this);
            set => Binding_SetName(this, value);
        }

        public override bool Equals(object other) {
            if (!(other is Object @object)) {
                return false;
            } else {
                return CompareObjects(this, @object);
            }
        }

        public override int GetHashCode() {
            return base.GetHashCode();
        }

        public override string ToString() => Name;

        public static void Destroy(Object obj) => Binding_Destroy(obj);

        public static implicit operator bool(Object obj) => Binding_IsNativeAlive(obj);
        public static bool operator ==(Object a, Object b) => CompareObjects(a, b);
        public static bool operator !=(Object a, Object b) => !CompareObjects(a, b);

        private static bool CompareObjects(Object a, Object b) {
            bool aIsNull = a is null;
            bool bIsNull = b is null;
            bool bothAreNull = aIsNull && bIsNull;
            bool result;
            if (bothAreNull) {
                result = true;
            } else {
                if (bIsNull) {
                    result = !Binding_IsNativeAlive(a);
                } else {
                    if (aIsNull) {
                        result = !Binding_IsNativeAlive(b);
                    } else {
                        result = ReferenceEquals(a, b);
                    }
                }
            }
            return result;
        }

        [MethodImpl(MethodImplOptions.InternalCall)] private static extern string Binding_GetName(Object @this);
        [MethodImpl(MethodImplOptions.InternalCall)] private static extern void Binding_SetName(Object @this, string name);
        [MethodImpl(MethodImplOptions.InternalCall)] private static extern void Binding_Destroy(Object @this);
        [MethodImpl(MethodImplOptions.InternalCall)] private static extern bool Binding_IsNativeAlive(Object @this);
    }
}
