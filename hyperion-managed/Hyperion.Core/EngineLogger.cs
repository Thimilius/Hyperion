namespace Hyperion {
  internal unsafe class EngineLogger : ILogger {
    public void Trace(string message) => Bindings.Log.Trace(message);
    public void Info(string message) => Bindings.Log.Info(message);
    public void Warn(string message) => Bindings.Log.Warn(message);
    public void Error(string message) => Bindings.Log.Error(message);
  }
}