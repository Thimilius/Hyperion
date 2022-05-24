﻿using System.Runtime.InteropServices;

namespace Hyperion {
  public class World : Object {
    public unsafe string Name {
      get => Marshal.PtrToStringUTF8(Bindings.World.GetName(NativeHandle));
      set => Bindings.World.SetName(NativeHandle, value);
    }
  }
}