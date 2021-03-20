using System;

namespace Hyperion.Editor {
    public static class Application {
        public static void Update() {
            Engine.Log("Hello there from managed code! {0}", Time.ElapsedTime);
        }
    }
}
