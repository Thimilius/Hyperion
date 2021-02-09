using System;

namespace Hyperion {
    public static class Application {
        private static Entity m_Entity;

        public static void Start() {
            Console.WriteLine("Start");
        }

        public static void Update() {
            if (Input.IsKeyDown(KeyCode.N)) {
                if (!m_Entity) {
                    m_Entity = Entity.CreatePrimitive(EntityPrimitive.Cube);
                }
            }

            if (m_Entity) {
                Transform transform = m_Entity.Transform;
                Vector3 position = transform.Position;
                position.x += Time.DeltaTime;
                transform.Position = position;
            }

            if (Input.IsKeyDown(KeyCode.Delete)) {
                Object.Destroy(m_Entity);
            }
        }
    }
}
