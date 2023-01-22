using System.Security.Policy;

namespace highlo
{
    public class Application
    {
        public static uint Width
        {
            get
            {
                return InternalCalls.Application_GetWidth();
            }
            set
            {
                InternalCalls.Application_SetWidth(value);
            }
        }

        public static uint Height
        {
            get
            {
                return InternalCalls.Application_GetHeight();
            }
            set
            {
                InternalCalls.Application_SetWidth(value);
            }
        }

        public static float AspectRatio => Width / (float)Height;

        public static void Quit()
        {
            InternalCalls.Application_Quit();
        }
    }
}

