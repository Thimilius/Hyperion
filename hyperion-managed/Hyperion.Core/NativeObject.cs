using System;

namespace Hyperion {
  public class NativeObject {
    internal IntPtr NativeHandle;

    public bool IsAlive => NativeHandle != IntPtr.Zero;
    
    protected NativeObject() {
                        
    }
  }
}