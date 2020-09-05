using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace WrecklessScript.Core
{
    namespace Input
    {
		[StructLayout(LayoutKind.Sequential)]
		public struct MouseCoords
		{
			public int X;
			public int Y;
		};

		public static class Mouse
        {
			[MethodImpl(MethodImplOptions.InternalCall)]
			public static extern void SetPosition(int x, int y);
			[MethodImpl(MethodImplOptions.InternalCall)]
			public static extern int GetPositionX();
			[MethodImpl(MethodImplOptions.InternalCall)]
			public static extern int GetPositionY();
			[MethodImpl(MethodImplOptions.InternalCall)]
			public static extern MouseCoords GetPosition();
			[MethodImpl(MethodImplOptions.InternalCall)]
			public static extern bool IsLeftMouseDown();
			[MethodImpl(MethodImplOptions.InternalCall)]
			public static extern bool IsRightMouseDown();
		}
    }
}
