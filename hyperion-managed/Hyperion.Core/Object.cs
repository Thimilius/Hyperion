using System;

namespace Hyperion {
  public class Object {
    internal IntPtr NativeHandle;

    public bool IsAlive => NativeHandle != IntPtr.Zero;
    
    protected Object() {
                        
    }
  }
}