using System;
using System.Runtime.InteropServices;


namespace WrecklessScript.Core.Math
{
    [StructLayout(LayoutKind.Explicit)]
    public struct Matrix4
    {
        [FieldOffset(0)]  public float D11;
        [FieldOffset(4)]  public float D12;
        [FieldOffset(8)]  public float D13;
        [FieldOffset(12)] public float D14;

        [FieldOffset(16)] public float D21;
        [FieldOffset(20)] public float D22;
        [FieldOffset(24)] public float D23;
        [FieldOffset(28)] public float D24;

        [FieldOffset(32)] public float D31;
        [FieldOffset(36)] public float D32;
        [FieldOffset(40)] public float D33;
        [FieldOffset(44)] public float D34;

        [FieldOffset(48)] public float D41;
        [FieldOffset(52)] public float D42;
        [FieldOffset(56)] public float D43;
        [FieldOffset(60)] public float D44;

        public Matrix4(float value)
        {
            D11 = value; D12 = 0.0f; D13 = 0.0f; D14 = 0.0f;
            D21 = 0.0f; D22 = value; D23 = 0.0f; D24 = 0.0f;
            D31 = 0.0f; D32 = 0.0f; D33 = value; D34 = 0.0f;
            D41 = 0.0f; D42 = 0.0f; D43 = 0.0f; D44 = value;
        }

        public Vector3 Translation
        {
            get { return new Vector3(D41, D42, D43); }
            set { D41 = value.X; D42 = value.Y; D43 = value.Z; }
        }

        public static Matrix4 Translate(Vector3 translation)
        {
            Matrix4 result = new Matrix4(1.0f);
            result.D41 = translation.X;
            result.D42 = translation.Y;
            result.D43 = translation.Z;
            return result;
        }

        public static Matrix4 Scale(Vector3 scale)
        {
            Matrix4 result = new Matrix4(1.0f);
            result.D41 = scale.X;
            result.D42 = scale.Y;
            result.D43 = scale.Z;
            return result;
        }

        public static Matrix4 Scale(float scale)
        {
            Matrix4 result = new Matrix4(1.0f);
            result.D41 = scale;
            result.D42 = scale;
            result.D43 = scale;
            return result;
        }

        public void DebugPrint()
        {
            Debug.Log($"{D11}, {D12}, {D13}, {D14}");
            Debug.Log($"{D21}, {D22}, {D23}, {D24}");
            Debug.Log($"{D31}, {D32}, {D33}, {D34}");
            Debug.Log($"{D41}, {D42}, {D43}, {D44}");
        }
    }
}
