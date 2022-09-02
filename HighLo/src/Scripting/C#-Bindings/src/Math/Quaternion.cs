using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace highlo
{
    public class Quaternion
    {
        public float W;
        public float X;
        public float Y;
        public float Z;

        public Quaternion(float scalar)
        {
            X = scalar;
            Y = scalar;
            Z = scalar;
            W = scalar;
        }

        public Quaternion(ref Vector3 v, float scalar)
        {
            X = v.X;
            Y = v.Y;
            Z = v.Z;
            W = scalar;
        }

        public Quaternion(ref Quaternion other)
        {
            W = other.W;
            X = other.X;
            Y = other.Y;
            Z = other.Z;
        }


    }
}

