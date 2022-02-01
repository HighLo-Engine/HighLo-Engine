package en.highlo.statistics.utils;

public class StringUtils
{
    public static String convertToForwardSlash(String path)
    {
        return path.replaceAll("\\\\", "/");
    }

    public static String convertToBackwardSlash(String path)
    {
        return path.replaceAll("/", "\\\\");
    }

    public static String removeWhiteSpace(String str)
    {
        return str.replaceAll("\\s+", "");
    }
}
