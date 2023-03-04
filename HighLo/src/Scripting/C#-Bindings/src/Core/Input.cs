namespace highlo
{
    public class Input
    {
        public static bool IsKeyPressed(KeyCode keyCode)
        {
            return InternalCalls.Input_IsKeyPressed(keyCode);
        }

        public static bool IsMouseButtonPressed(MouseButton button)
        {
            return InternalCalls.Input_IsMouseButtonPressed(button);
        }

        public static Vector2 GetMousePosition()
        {
            InternalCalls.Input_GetMousePosition(out Vector2 result);
            return result;
        }

        public static bool IsControllerPresent(int id)
        {
            return InternalCalls.Input_IsControllerPresent(id);
        }

        public static int[] GetAllConnectedControllerIds()
        {
            return InternalCalls.Input_GetControllerIds();
        }

        public static string GetControllerName(int id)
        {
            return InternalCalls.Input_GetControllerName(id);
        }

        public static bool IsControllerButtonPressed(int id, GamepadButton button)
        {
            return InternalCalls.Input_IsControllerButtonPressed(id, (int)button);
        }

        public static bool IsControllerButtonPressed(int id, int button)
        {
            return InternalCalls.Input_IsControllerButtonPressed(id, button);
        }

        public static float GetCurrentControllerAxisValue(int id, int axis)
        {
            return InternalCalls.Input_GetControlerAxis(id, axis);
        }

        public static byte GetCurrentControllerHatValue(int id, int hat)
        {
            return InternalCalls.Input_GetControllerHat(id, hat);
        }

        public static void SetCursorMode(CursorMode mode)
        {
            InternalCalls.Input_SetCursorMode(mode);
        }

        public static CursorMode GetCurrentCursorMode()
        {
            return InternalCalls.Input_GetCursorMode();
        }
    }
}

