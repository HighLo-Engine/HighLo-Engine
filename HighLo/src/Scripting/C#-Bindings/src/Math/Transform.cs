namespace highlo
{
    public class Transform
    {
        private Vector3 m_Position;
        private Vector3 m_Scale;
        private Quaternion m_Rotation;

        public Transform()
        {
            m_Position = new Vector3(0.0f);
            m_Scale = new Vector3(1.0f);
            m_Rotation = new Quaternion(new Vector3(0.0f), 1.0f);
        }

        public Transform(ref Transform other)
        {
            m_Position = other.m_Position;
            m_Scale = other.m_Scale;
            m_Rotation = other.m_Rotation;
        }

        public void Translate(ref Vector3 translation)
        {

        }

        public void Rotate(float angleDegrees, ref Vector3 axis)
        {

        }

        public void Rotate(ref Quaternion rotation)
        {

        }

        public void Scale(ref Vector3 scale)
        {

        }

        public Vector3 GetPosition()
        {
            return m_Position;
        }

        public Vector3 GetScale()
        {
            return m_Scale;
        }

        public Quaternion GetRotation()
        {
            return m_Rotation;
        }
    }
}
