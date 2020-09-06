using System;
using System.Runtime.CompilerServices;
namespace WrecklessScript.Core
{
    public static class Debug
    {
        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern void Log(string message);

        public static void Log(string message, params object[] objects)
        {
            string _FinalMessage = message;
            foreach (var obj in objects)
                _FinalMessage += obj.ToString();
            Log(_FinalMessage);
        }

        public static void Log(params object[] objects)
        {
            string _FinalMessage = "";
            foreach (var obj in objects)
                _FinalMessage += obj.ToString();
            Log(_FinalMessage);
        }

        public static void Assert(bool condition)
        {
            Assert(condition, null);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern void Assert(bool condition, string message);
    }
}
