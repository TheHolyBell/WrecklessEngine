using System;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace WrecklessScript.Core
{
    namespace Input
    {
        public static class GamePad
        {
			[StructLayout(LayoutKind.Sequential)]
			public struct StickCoords
            {
				public float X;
				public float Y;
            }

			[StructLayout(LayoutKind.Sequential)]
			public struct VibrationState
            {
				public int LeftMotor;
				public int RightMotor;
            }

			[MethodImpl(MethodImplOptions.InternalCall)]
			public static extern bool IsPressed(Button btn);

			[MethodImpl(MethodImplOptions.InternalCall)]
			public static extern float LeftTrigger();

			[MethodImpl(MethodImplOptions.InternalCall)]
			public static extern float RightTrigger();


			public static extern VibrationState Vibration
            {
				[MethodImpl(MethodImplOptions.InternalCall)]
				get;
				[MethodImpl(MethodImplOptions.InternalCall)]
				set;
            }

			[MethodImpl(MethodImplOptions.InternalCall)]
			public static extern StickCoords LeftStick();

			[MethodImpl(MethodImplOptions.InternalCall)]
			public static extern StickCoords RightStick();

			public static extern int Sensitivity
            {
				[MethodImpl(MethodImplOptions.InternalCall)]
				get;
				[MethodImpl(MethodImplOptions.InternalCall)]
				set;
            }
		}
    }
}
