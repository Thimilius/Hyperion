using System;

namespace Hyperion {
    public static class Application {
        private static Entity m_Entity;

        public static void Start() {

        }

        public static void Update() {
            if (Input.IsKeyDown(KeyCode.N)) {
                if (!m_Entity) {
                    m_Entity = Entity.CreatePrimitive(EntityPrimitive.Cube);
                } else {

                }
            }
            
            if (Input.IsKeyDown(KeyCode.Delete)) {
                Object.Destroy(m_Entity);
            }
        }
    }
}
