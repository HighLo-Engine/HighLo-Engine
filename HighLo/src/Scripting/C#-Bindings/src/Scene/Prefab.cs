namespace highlo
{
    public class Prefab
    {
        internal AssetHandle m_Handle;

        public AssetHandle Handle => m_Handle;

        internal Prefab()
        {
            m_Handle = AssetHandle.Invalid;
        }

        internal Prefab(AssetHandle handle)
        {
            m_Handle = handle;
        }
    }
}
