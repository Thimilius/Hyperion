using System;

namespace Hyperion {
    public class HyperionException : Exception {
        public HyperionException() { }
        public HyperionException(string message) : base(message) { }
        public HyperionException(string message, Exception innerException) : base(message, innerException) { }
    }
}
