using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace highlo
{
    public class Vector4
    {
        public float X;
        public float Y;
        public float Z;
        public float W;

        public Vector4(float scalar)
        {
            X = scalar;
            Y = scalar;
            Z = scalar;
            W = scalar;
        }

        public Vector4(float x, float y, float z, float w)
        {
            X = x;
            Y = y;
            Z = z;
            W = w;
        }

        public Vector4(ref Vector2 other)
        {
            X = other.X;
            Y = other.Y;
        }

        public Vector4(ref Vector3 other)
        {
            X = other.X;
            Y = other.Y;
            Z = other.Z;
        }

        public Vector4(ref Vector4 other)
        {
            X = other.X;
            Y = other.Y;
            Z = other.Z;
            W = other.W;
        }


    }
}

