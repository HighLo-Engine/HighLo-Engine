using System.Collections.Generic;

namespace highlo
{
    public class Scene
    {
        private static Dictionary<int, Entity> s_EntityCache = new Dictionary<int, Entity>();

        public static Entity[] GetEntities()
        {
            return InternalCalls.Scene_GetEntities();
        }

        public static Entity CreateEntity(string tag = "Unnamed Entity")
        {
            return new Entity(InternalCalls.Scene_CreateEntity(tag));
        }

        public static void DestroyEntity(Entity entity)
        {
            if (entity == null)
                return;

            if (!InternalCalls.Scene_IsEntityValid(entity.ID))
                return;

            int hash = entity.ID.GetHashCode();

            if (!s_EntityCache.Remove(hash))
                s_EntityCache.Remove(entity.Tag.GetHashCode());

            InternalCalls.Scene_DestroyEntity(entity.ID);
        }

        public static Entity FindEntityByTag(string tag)
        {
            int hash = tag.GetHashCode();

            if (s_EntityCache.ContainsKey(hash) && s_EntityCache[hash] != null)
            {
                Entity entity = s_EntityCache[hash];
                if (!InternalCalls.Scene_IsEntityValid(entity.ID))
                {
                    s_EntityCache.Remove(hash);
                    entity = null;
                }

                return entity;
            }

            ulong entityID = InternalCalls.Scene_FindEntityByTag(tag);
            Entity newEntity = entityID != 0 ? new Entity(entityID) : null;
            s_EntityCache[hash] = newEntity;

            return newEntity;
        }

        public static Entity FindEntityByUUID(ulong uuid)
        {
            int hash = uuid.GetHashCode();
            if (s_EntityCache.ContainsKey(hash) && s_EntityCache[hash] != null)
            {
                Entity entity = s_EntityCache[hash];
                if (InternalCalls.Scene_IsEntityValid(entity.ID))
                {
                    s_EntityCache.Remove(hash);
                    entity = null;
                }

                return entity;
            }

            if (!InternalCalls.Scene_IsEntityValid(uuid))
                return null;

            Entity newEntity = new Entity(uuid);
            s_EntityCache[hash] = newEntity;
            return newEntity;
        }
    }
}
