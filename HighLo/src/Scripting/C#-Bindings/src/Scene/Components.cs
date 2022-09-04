namespace highlo
{
    public abstract class Component
    {
        public Entity Entity { get; internal set; }
    }

    public class ScriptComponent : Component
    {
        public object Instance => InternalCalls.ScriptComponent_GetInstance(Entity.ID);
    }

    public class PrefabComponent : Component
    {

    }
}
