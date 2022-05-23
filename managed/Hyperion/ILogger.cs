namespace Hyperion {
  public interface ILogger {
    void Info(string message);
    void Trace(string message);
    void Warn(string message);
    void Error(string message);
  }
}