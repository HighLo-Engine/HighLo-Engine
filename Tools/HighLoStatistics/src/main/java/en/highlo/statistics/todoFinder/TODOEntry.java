package en.highlo.statistics.todoFinder;

import java.util.List;

public class TODOEntry
{
    /// <summary>
    /// Determines the FilePath to the current Entry file.
    /// </summary>
    public String FilePath;

    /// <summary>
    /// Determines the total count of ToDos found in the current Entry file.
    /// </summary>
    public int ToDoCount = 0;

    /// <summary>
    /// Determines all lines of the current file, that contain a ToDo.
    /// </summary>
    public List<String> ToDoLines;
}
