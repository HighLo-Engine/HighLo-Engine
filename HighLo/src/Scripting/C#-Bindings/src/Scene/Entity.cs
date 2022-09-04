using System;
using System.Collections.Generic;

namespace highlo
{
    public class Entity
    {
        public readonly ulong ID;
        public string Tag => InternalCalls.Entity_GetTag(ID);

        private Entity m_Parent;
        private Dictionary<Type, Component> ComponentCache = new Dictionary<Type, Component>();

        public Vector3 Translation
        {
            get
            {
                return InternalCalls.Entity_GetTransformOf(ID).GetPosition();
            }
            set
            {
                InternalCalls.Entity_SetTransform_Position(ID, value);
            }
        }

        public Vector3 Scale
        {
            get
            {
                return InternalCalls.Entity_GetTransformOf(ID).GetScale();
            }
            set
            {
                InternalCalls.Entity_SetTransform_Scale(ID, value);
            }
        }

        public Quaternion Rotation
        {
            get
            {
                return InternalCalls.Entity_GetTransformOf(ID).GetRotation();
            }
            set
            {
                InternalCalls.Entity_SetTransform_Rotation(ID, value);
            }
        }

        public Entity Parent
        {
            get
            {
                ulong parentID = InternalCalls.Entity_GetParentOf(ID);
                if (m_Parent == null || m_Parent.ID != parentID)
                {
                    m_Parent = InternalCalls.Entity_IsValid(parentID) ? new Entity(parentID) : null;
                }

                return m_Parent;
            }
            set
            {
                InternalCalls.Entity_SetParent(ID, value.ID);
            }
        }

        public Entity[] Children => InternalCalls.Entity_GetChildrenOf(ID);

        public Entity()
        {
            ID = 0;
        }

        public Entity(ulong id)
        {
            ID = id;
        }

        public bool HasComponent<T>() where T : Component
        {
            return InternalCalls.Entity_HasComponent(ID, typeof(T));
        }

        public bool HasComponent(Type type)
        {
            return InternalCalls.Entity_HasComponent(ID, type);
        }

        public T GetComponent<T>() where T : Component, new()
        {
            Type componentType = typeof(T);
            if (!HasComponent<T>())
            {
                if (ComponentCache.ContainsKey(componentType))
                {
                    ComponentCache.Remove(componentType);
                    return null;
                }
            }

            if (!ComponentCache.ContainsKey(componentType))
            {
                T component = new T { Entity = this };
                ComponentCache.Add(componentType, component);
                return component;
            }

            return ComponentCache[componentType] as T;
        }

        public T AddComponent<T>() where T : Component, new()
        {
            if (HasComponent<T>())
                return GetComponent<T>();

            Type componentType = typeof(T);
            InternalCalls.Entity_AddComponent(ID, componentType);
            T component = new T { Entity = this };
            ComponentCache.Add(componentType, component);
            return component;
        }

        public bool RemoveComponent<T>() where T : Component
        {
            Type componentType = typeof(T);
            bool removed = InternalCalls.Entity_RemoveComponent(ID, componentType);

            if (removed && ComponentCache.ContainsKey(componentType))
            {
                ComponentCache.Remove(componentType);
            }

            return removed;
        }

        public bool Is<T>() where T : Component, new()
        {
            ScriptComponent sc = GetComponent<ScriptComponent>();
            object instance = sc?.Instance;
            return instance is T;
        }

        public T As<T>() where T : Component, new()
        {
            ScriptComponent sc = GetComponent<ScriptComponent>();
            object instance = sc?.Instance;
            return instance as T;
        }

        public void Destroy()
        {
            Scene.DestroyEntity(this);
        }

        public void Destroy(Entity other)
        {
            Scene.DestroyEntity(other);
        }
    }
}
