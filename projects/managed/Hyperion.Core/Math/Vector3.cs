namespace Hyperion {
    public struct Vector3 {
        public float x;
        public float y;
        public float z;

        public Vector3(float x, float y, float z) {
            this.x = x;
            this.y = y;
            this.z = z;
        }

        public override string ToString() {
            return $"({x:0.00}, {y:0.00}, {z:0.00})";
        }
    }
}
