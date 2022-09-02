using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace highlo
{
    public class Vector3
    {
        public float X;
        public float Y;
        public float Z;

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
        public Vector3(ref Vector2 other)
        {
            X = other.X;
            Y = other.Y;
            Z = 0.0f;
        }

        public Vector3(ref Vector3 other)
        {
            X = other.X;
            Y = other.Y;
            Z = other.Z;
        }

        public Vector3(ref Vector4 other)
        {
            X = other.X;
            Y = other.Y;
            Z = other.Z;
        }


    }
}

