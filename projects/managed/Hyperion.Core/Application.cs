using System.Collections.Generic;

namespace Hyperion {
    public static class Application {
        class MyScript : Script {
            protected override void OnCreate() {

            }

            protected override void OnUpdate(float deltaTime) {

            }

            protected override void OnDestroy() {

            }
        }

        class MyComponent : Component {

        }

        private static readonly List<Entity> m_Entities = new List<Entity>();

        public static void Start() {
            
        }

        public static void Update() {
            if (Input.IsKeyDown(KeyCode.N)) {
                if (m_Entities.Count == 0) {
                    for (int x = 0; x < 32; x++) {
                        for (int z = 0; z < 32; z++) {
                            Entity e = Entity.CreatePrimitive(EntityPrimitive.Cube);
                            e.Transform.Position = new Vector3(x * 2, 0, z * 2);
                            e.AddComponent<MyScript>();
                            e.AddComponent<MyComponent>();
                            m_Entities.Add(e);
                        }
                    }
                }
            }

            if (Input.IsKeyDown(KeyCode.Delete)) {
                foreach (Entity entity in m_Entities) {
                    Object.Destroy(entity);
                }
                m_Entities.Clear();
            }
        }
    }
}
