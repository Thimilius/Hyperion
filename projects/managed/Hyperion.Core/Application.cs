using System;

namespace Hyperion {
    public static class Application {
        private static Entity m_Entity;

        class MyScript : Script {
            public string MyString { get; set; }
        }

        public static void Start() {

        }

        public static void Update() {
            if (Input.IsKeyDown(KeyCode.N)) {
                if (!m_Entity) {
                    m_Entity = Entity.CreatePrimitive(EntityPrimitive.Cube);
                    MyScript script = m_Entity.AddComponent<MyScript>();
                    script.MyString = "Hello there";
                }
            }

            if (m_Entity != null) {
                MyScript myScript = m_Entity.GetComponent<MyScript>();
                if (myScript != null) {
                    Engine.Log(myScript.MyString);
                } else {
                    Engine.Log("No Script");
                }

                if (Input.IsKeyDown(KeyCode.Delete)) {
                    Object.Destroy(m_Entity.GetComponent<MyScript>());
                }
            }
        }
    }
}
