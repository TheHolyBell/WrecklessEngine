using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using WrecklessScript.Core.Math;

namespace WrecklessScript.Core
{
    public abstract class Component
    {
        public Entity Entity { get; set; }
    }

    public class TagComponent : Component
    {
        public string Tag
        {
            get => GetTag_Native(Entity.ID);

            set => SetTag_Native(Entity.ID, value);
        }
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern string GetTag_Native(uint entityID);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetTag_Native(uint entityID, string value);
    }

    public class TransformComponent : Component
    {
        public Matrix4 Transform
        {
            get
            {
                Matrix4 _Result;
                GetTransform_Native(Entity.ID, out _Result);
                return _Result;
            }
            set => SetTransform_Native(Entity.ID, ref value);

        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void GetTransform_Native(uint entityID, out Matrix4 result);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetTransform_Native(uint entityID, ref Matrix4 result);
    }
}
