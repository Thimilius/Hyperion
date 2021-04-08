namespace Hyperion {
    public struct Quaternion {
        public float x;
        public float y;
        public float z;
        public float w;

        public static Quaternion Identity => new Quaternion(0.0f, 0.0f, 0.0f, 1.0f);

        public Quaternion(float x, float y, float z, float w) {
            this.x = x;
            this.y = y;
            this.z = z;
            this.w = w;
        }

        public override string ToString() {
            return $"({x:0.00}, {y:0.00}, {z:0.00}, {w:0.00})";
        }
    }
}
