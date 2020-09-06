using System;
using System.Runtime.CompilerServices;

namespace WrecklessScript.Core
{
    public static class Time
    {
        // @ The completion time (in seconds) since the last frame
        public static extern float DeltaTime {
            [MethodImpl(MethodImplOptions.InternalCall)]
            get;
        }
        // @ Count of frames per second
        public static extern int FrameCount
        {
            [MethodImpl(MethodImplOptions.InternalCall)]
            get;
        }
        // @ Time (in seconds) since the start of game
        public static extern float TotalTime
        {
            [MethodImpl(MethodImplOptions.InternalCall)]
            get;
        }

        // @ Time scale (can be used for effects such as: slow motion, etc.)
        public static extern float TimeScale
        {
            [MethodImpl(MethodImplOptions.InternalCall)]
            get;
            [MethodImpl(MethodImplOptions.InternalCall)]
            set;
        }
    }
}
