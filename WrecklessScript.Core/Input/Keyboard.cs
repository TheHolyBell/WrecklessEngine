using System;
using System.Runtime.CompilerServices;

namespace WrecklessScript.Core
{
    namespace Input
    {
        public static class Keyboard
        {
            [MethodImpl(MethodImplOptions.InternalCall)]
            public static extern bool CapsLock();
            [MethodImpl(MethodImplOptions.InternalCall)]
            public static extern bool NumLock();
            [MethodImpl(MethodImplOptions.InternalCall)]
            public static extern bool IsKeyDown(KeyCode key);
            [MethodImpl(MethodImplOptions.InternalCall)]
            public static extern bool IsKeyUp(KeyCode key);
        }
    }
}
