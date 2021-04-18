namespace Hyperion {
    public class ObjectDestroyedException : HyperionException {
        public ObjectDestroyedException() : base("The object you are trying to access was already destroyed") { }
    }
}
