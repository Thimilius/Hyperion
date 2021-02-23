using System;

namespace Hyperion.Editor {
    public static class Application {
        class MyScript : Script {
            protected override void OnCreate() {
            }

            protected override void OnUpdate(float deltaTime) {
                float value = Time.ElapsedTime * 25.0f;
                Transform.EulerAngles = new Vector3(value, value, 0);
            }

            protected override void OnDestroy() {
            }
        }

        class MyComponent : Component {

        }

        private static Entity s_Parent;

        public static void Update() {
            if (Input.IsKeyDown(KeyCode.N)) {
                if (!s_Parent) {
                    s_Parent = new Entity();

                    for (int x = 0; x < 10; x++) {
                        for (int z = 0; z < 10; z++) {
                            Entity entity = Entity.CreatePrimitive(EntityPrimitive.Cube);
                            entity.Transform.Parent = s_Parent.Transform;
                            entity.Transform.Position = new Vector3(x * 2, 0, z * 2);
                            entity.AddComponent<MyScript>();
                            entity.AddComponent<MyComponent>();
                        }
                    }
                }
            }

            if (s_Parent) {
                s_Parent.Transform.Position = new Vector3((float)Math.Sin(Time.ElapsedTime) * 10.0f, 0, 0);
                s_Parent.Transform.EulerAngles = new Vector3(0, Time.ElapsedTime * 10.0f, 0);
            }

            if (Input.IsKeyDown(KeyCode.Delete)) {
                Object.Destroy(s_Parent);
            }
        }
    }
}
