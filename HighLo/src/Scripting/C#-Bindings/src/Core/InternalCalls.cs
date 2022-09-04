using System;
using System.Runtime.CompilerServices;

namespace highlo
{
    internal class InternalCalls
    {
        #region Application

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern uint Application_GetWidth();
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Application_SetWidth(uint width);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern uint Application_GetHeight();
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Application_SetHeight(uint height);
        
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Application_Quit();

        #endregion

        #region Entity

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern bool Entity_IsValid(ulong entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern string Entity_GetTag(ulong entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Entity_SetTag(ulong entityID, string tag);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern ulong Entity_GetParentOf(ulong parentID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Entity_SetParent(ulong entityID, ulong parentID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern Entity[] Entity_GetChildrenOf(ulong entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Entity_AddComponent(ulong entityID, Type componentType);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern bool Entity_RemoveComponent(ulong entityID, Type componentType);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern bool Entity_HasComponent(ulong entityID, Type componentType);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern Transform Entity_GetTransformOf(ulong entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Entity_SetTransform_Position(ulong entityID, Vector3 position);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Entity_SetTransform_Scale(ulong entityID, Vector3 scale);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Entity_SetTransform_Rotation(ulong entityID, Quaternion rotation);

        #endregion

        #region Scene

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern bool Scene_IsEntityValid(ulong entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern ulong Scene_CreateEntity(string tag);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Scene_DestroyEntity(ulong entityID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern Entity[] Scene_GetEntities();

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern ulong Scene_FindEntityByTag(string tag);

        #endregion

        #region ScriptComponent

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern object ScriptComponent_GetInstance(ulong entityID);

        #endregion

        #region Input

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern bool Input_IsKeyPressed(KeyCode keyCode);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern bool Input_IsMouseButtonPressed(MouseButton button);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Input_GetMousePosition(out Vector2 position);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Input_SetCursorMode(CursorMode cursorMode);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern CursorMode Input_GetCursorMode();

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern bool Input_IsControllerPresent(int id);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern int[] Input_GetControllerIds();

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern string Input_GetControllerName(int id);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern bool Input_IsControllerButtonPressed(int id, int button);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern float Input_GetControlerAxis(int id, int axis);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern byte Input_GetControllerHat(int id, int hat);

        #endregion

        #region Log

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void Log_LogMessage(Log.LogLevel logLevel, string msg);

        #endregion

        #region AssetHandle

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern bool AssetHandle_IsValid(ref AssetHandle handle);

        #endregion
    }
}

