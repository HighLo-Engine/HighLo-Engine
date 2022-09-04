namespace highlo
{
    public struct Vector2
    {
        public float X;
        public float Y;

        public Vector2(float scalar)
        {
            X = scalar;
            Y = scalar;
        }

        public Vector2(float x, float y)
        {
            X = x;
            Y = y;
        }

        public Vector2(ref Vector2 other)
        {
            X = other.X;
            Y = other.Y;
        }

        public Vector2(ref Vector3 other)
        {
            X = other.X;
            Y = other.Y;
        }

        public Vector2(ref Vector4 other)
        {
            X = other.X;
            Y = other.Y;
        }


    }
}

