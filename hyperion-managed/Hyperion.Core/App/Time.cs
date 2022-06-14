namespace Hyperion {
  public static class Time {
    public static unsafe float ElapsedTime => Bindings.Time.GetTime();
    public static unsafe float DeltaTime => Bindings.Time.GetDeltaTime();
  }
}