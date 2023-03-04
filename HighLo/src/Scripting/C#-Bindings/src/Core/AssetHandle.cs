using System.Runtime.InteropServices;

namespace highlo
{
    [StructLayout(LayoutKind.Sequential)]
    public struct AssetHandle
    {
        public static readonly AssetHandle Invalid = new AssetHandle(0);

        private ulong m_ID;

        public AssetHandle(ulong uuid)
        {
            m_ID = uuid;
        }

        public bool IsValid()
        {
            return InternalCalls.AssetHandle_IsValid(ref this);
        }

        public static implicit operator bool(AssetHandle handle)
        {
            return InternalCalls.AssetHandle_IsValid(ref handle);
        }

        public override string ToString()
        {
            return m_ID.ToString();
        }
    }
}
