namespace Hyperion {
  public static class Input {
    public static unsafe bool IsKeyDown(KeyCode keyCode) => Bindings.Input.IsKeyDown(keyCode);
    public static unsafe bool IsKeyHold(KeyCode keyCode) => Bindings.Input.IsKeyHold(keyCode);
    public static unsafe bool IsKeyUp(KeyCode keyCode) => Bindings.Input.IsKeyUp(keyCode);
  }
}