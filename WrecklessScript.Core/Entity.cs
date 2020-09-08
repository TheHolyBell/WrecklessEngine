using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace WrecklessScript.Core
{
    public abstract class Entity
    {
        public uint ID { get; private set; }
        public abstract void Update();
        public abstract void Start();

        protected Entity() { ID = 0; }

        internal Entity(uint id)
        {
            ID = id;
        }

        public T AddComponent<T>() where T : Component, new()
        {
            AddComponent_Native(ID, typeof(T));
            T _Component = new T();
            _Component.Entity = this;
            return _Component;
        }

        public bool HasComponent<T>() where T : Component, new()
        {
            return HasComponent_Native(ID, typeof(T));
        }

        public T GetComponent<T>() where T : Component, new()
        {
            if(HasComponent<T>())
            {
                T _Component = new T();
                _Component.Entity = this;
                return _Component;
            }

            return null;
        }

        public void RemoveComponent<T>() where T : Component
        {
            RemoveComponent_Native(ID, typeof(T));
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void AddComponent_Native(uint entityID, Type type);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern bool HasComponent_Native(uint entityID, Type type);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void RemoveComponent_Native(uint entityID, Type type);
    }
}
