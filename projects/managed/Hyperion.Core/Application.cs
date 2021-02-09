using System;

namespace Hyperion {
    public static class Application {
        public static void Start() {
            Console.WriteLine("Start");
        }

        public static void Update() {
            if (Input.IsKeyDown(KeyCode.H)) {
                Console.WriteLine("Hold your horses!");
            }
        }
    }
}
