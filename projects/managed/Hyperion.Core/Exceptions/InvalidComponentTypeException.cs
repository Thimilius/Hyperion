namespace Hyperion {
    public class InvalidComponentTypeException : HyperionException {
        public InvalidComponentTypeException() : base("A managed scripting component must inherit from 'Hyperion.Script'") { }
    }
}
