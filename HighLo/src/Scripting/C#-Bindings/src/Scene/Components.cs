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

    public class CameraComponent : Component
    {

    }

    public class StaticModelComponent : Component
    {

    }

    public class DynamicModelComponent : Component
    {

    }

    public class DirectionalLightComponent : Component
    {

    }

    public class PointLightComponent : Component
    {

    }

    public class SkyLightComponent : Component
    {

    }

    public class TextComponent : Component
    {

    }
}
