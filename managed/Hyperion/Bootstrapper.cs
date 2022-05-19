using System;
using System.Runtime.InteropServices;

namespace Hyperion
{
  internal static class Bootstrapper
  {
    [StructLayout(LayoutKind.Sequential)]
    private struct LibArgs
    {
      public IntPtr Message;
      public int Number;
      public unsafe delegate *unmanaged<int, string, int> Function;
    }

    internal static int Bootstrap(IntPtr arg, int argLength)
    {
      if (argLength < Marshal.SizeOf(typeof(LibArgs)))
      {
        return 1;
      }

      LibArgs libArgs = Marshal.PtrToStructure<LibArgs>(arg);
      Console.WriteLine($"Hello, world! from {nameof(Bootstrapper)}");
      PrintLibArgs(libArgs);
      return 0;
    }

    private static void PrintLibArgs(LibArgs libArgs)
    {
      string message = RuntimeInformation.IsOSPlatform(OSPlatform.Windows) ? Marshal.PtrToStringUni(libArgs.Message) : Marshal.PtrToStringUTF8(libArgs.Message);

      Console.WriteLine($"-- message: {message}");
      Console.WriteLine($"-- number: {libArgs.Number}");

      unsafe
      {
        Console.WriteLine($"Function Result: {libArgs.Function(13, "Hello there")}");
      }
    }
  }
}