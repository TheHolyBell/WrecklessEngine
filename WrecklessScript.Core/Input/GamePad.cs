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

			[MethodImpl(MethodImplOptions.InternalCall)]
			public static extern void SetVibration(int leftMotor, int rightMotor);

			[MethodImpl(MethodImplOptions.InternalCall)]
			public static extern VibrationState GetVibration();

			[MethodImpl(MethodImplOptions.InternalCall)]
			public static extern StickCoords LeftStick();

			[MethodImpl(MethodImplOptions.InternalCall)]
			public static extern StickCoords RightStick();

			[MethodImpl(MethodImplOptions.InternalCall)]
			public static extern void SetSensitivity(int sensitivity);

			[MethodImpl(MethodImplOptions.InternalCall)]
			public static extern int GetSensitivity();
		}
    }
}
