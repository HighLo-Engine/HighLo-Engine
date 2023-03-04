using System;

namespace highlo
{
    public class Vector3
    {
        public float X;
        public float Y;
        public float Z;

        public static Vector3 Zero = new Vector3(0.0f);
        public static Vector3 One = new Vector3(1.0f);
        public static Vector3 Infinity = new Vector3(float.PositiveInfinity);
        public static Vector3 Forward = new Vector3(0.0f, 0.0f, -1.0f);
        public static Vector3 Backward = new Vector3(0.0f, 0.0f, 1.0f);
        public static Vector3 Right = new Vector3(1.0f, 0.0f, 0.0f);
        public static Vector3 Left = new Vector3(-1.0f, 0.0f, 0.0f);
        public static Vector3 Up = new Vector3(0.0f, 1.0f, 0.0f);
        public static Vector3 Down = new Vector3(0.0f, -1.0f, 0.0f);

        public Vector2 XY
        {
            get
            {
                return new Vector2(X, Y);
            }
            set
            {
                X = value.X;
                Y = value.Y;
            }
        }

        public Vector2 YZ
        {
            get
            {
                return new Vector2(Y, Z);
            }
            set
            {
                Y = value.X;
                Z = value.Y;
            }
        }

        public Vector2 XZ
        {
            get
            {
                return new Vector2(X, Z);
            }
            set
            {
                X = value.X;
                Z = value.Y;
            }
        }

        public Vector3(float scalar)
        {
            X = scalar;
            Y = scalar;
            Z = scalar;
        }

        public Vector3(float x, float y, float z)
        {
            X = x;
            Y = y;
            Z = z;
        }
        
        public Vector3(float x, Vector2 yz)
        {
            X = x;
            Y = yz.X;
            Z = yz.Y;
        }

        public Vector3(Vector2 other)
        {
            X = other.X;
            Y = other.Y;
            Z = 0.0f;
        }

        public Vector3(Vector3 other)
        {
            X = other.X;
            Y = other.Y;
            Z = other.Z;
        }

        public Vector3(Vector4 other)
        {
            X = other.X;
            Y = other.Y;
            Z = other.Z;
        }

        public void Apply(Func<double, double> func)
        {
            X = (float)func(X);
            Y = (float)func(Y);
            Z = (float)func(Z);
        }

        public Vector3 New(Func<double, double> func)
        {
            return new Vector3((float)func(X), (float)func(Y), (float)func(Z));
        }

        public void Clamp(Vector3 min, Vector3 max)
        {
            
        }

        public float Length()
        {
            return (float)Math.Sqrt(X * X + Y * Y + Z * Z);
        }

        public Vector3 Normalized()
        {
            float length = Length();
            return length > 0.0f ? New(v => v / length) : new Vector3(X, Y, Z);
        }

        public void Normalize()
        {
            float length = Length();
            Apply(v => v / length);
        }

        public static Vector3 operator*(Vector3 left, float scalar)
        {
            return new Vector3(left.X * scalar, left.Y * scalar, left.Z * scalar);
        }

        public static Vector3 operator*(float scalar, Vector3 right)
        {
            return new Vector3(scalar * right.X, scalar * right.Y, scalar * right.Z);
        }

        public static Vector3 operator*(Vector3 left, Vector3 right)
        {
            return new Vector3(left.X * right.X, left.Y * right.Y, left.Z * right.Z);
        }


    }
}

