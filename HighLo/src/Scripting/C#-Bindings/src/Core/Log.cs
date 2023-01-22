namespace highlo
{
    public static class Log
    {
        internal enum LogLevel
        {
            Trace = 1 << 0,
            Debug = 1 << 1,
            Info = 1 << 2,
            Warn = 1 << 3,
            Error = 1 << 4,
            Critical = 1 << 5
        }

        private static void StringType(LogLevel logLevel, string format, object[] parameters)
        {
            InternalCalls.Log_LogMessage(logLevel, string.Format(format, parameters));
        }
        
        private static void ObjectType(LogLevel logLevel, object value)
        {
            InternalCalls.Log_LogMessage(logLevel, value != null ? value.ToString() : "null");
        }

        public static void Trace(string format, params object[] parameters) => StringType(LogLevel.Trace, format, parameters);
        public static void Debug(string format, params object[] parameters) => StringType(LogLevel.Debug, format, parameters);
        public static void Info(string format, params object[] parameters) => StringType(LogLevel.Info, format, parameters);
        public static void Warn(string format, params object[] parameters) => StringType(LogLevel.Warn, format, parameters);
        public static void Error(string format, params object[] parameters) => StringType(LogLevel.Error, format, parameters);
        public static void Critical(string format, params object[] parameters) => StringType(LogLevel.Critical, format, parameters);

        public static void Trace(object value) => ObjectType(LogLevel.Trace, value);
        public static void Debug(object value) => ObjectType(LogLevel.Debug, value);
        public static void Info(object value) => ObjectType(LogLevel.Info, value);
        public static void Warn(object value) => ObjectType(LogLevel.Warn, value);
        public static void Error(object value) => ObjectType(LogLevel.Error, value);
        public static void Critical(object value) => ObjectType(LogLevel.Critical, value);

    }
}

