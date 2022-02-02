package en.highlo.statistics.todoFinder;

import en.highlo.statistics.utils.RecursiveDirectoryIterator;
import en.highlo.statistics.utils.StringUtils;

import java.io.File;
import java.util.ArrayList;
import java.util.List;

public class TODOFinder
{
    private RecursiveDirectoryIterator mIterator;
    private List<TODOEntry> mEntries;
    private int mEntryIndex = 0;

    public TODOFinder(String rootPath)
    {
        mIterator = new RecursiveDirectoryIterator(rootPath, null);
        mEntries = new ArrayList<>();
    }

    public TODOFinder(String rootPath, String[] excludeDirs)
    {
        mIterator = new RecursiveDirectoryIterator(rootPath, excludeDirs);
        mEntries = new ArrayList<>();
    }

    public void iterateAndCount()
    {
        int iteratedFiles = mIterator.getIteratedFiles();
        System.out.println(iteratedFiles + " files found in total, that contain a todo!");

        // Prepare the internal structure to cover the worst case of every file containing a todo
        for (int i = 0; i < iteratedFiles; ++i)
        {
            TODOEntry entry = new TODOEntry();
            entry.ToDoLines = new ArrayList<>();
            mEntries.add(entry);
        }

        // Register all needed callbacks
        mIterator.registerDirectoryCallback(this::onDirectory);
        mIterator.registerFileCallback(this::onFile);
        mIterator.registerLineCallback(this::onLine);

        // Start the iterator
        mIterator.iterate();
    }

    public List<TODOEntry> getAllEntries()
    {
        // Iterate over the internal structure and add all entries to the final result structure, which have data in them
        int totalTodoCount = 0;
        for (TODOEntry entry : mEntries)
        {
            if (entry.FilePath != null)
            {
                ++totalTodoCount;
            }
        }

        List<TODOEntry> result = new ArrayList<>();
        for (int i = 0; i < totalTodoCount; ++i)
        {
            result.add(mEntries.get(i));
        }
        return result;
    }

    private void onLine(File file, String line)
    {
        if (line.contains("TODO: "))
        {
            int todoCount = 0;
            int index = 0;
            for (int i = 0; i < mEntries.size(); ++i)
            {
                TODOEntry entry = mEntries.get(i);
                if (entry.FilePath != null && entry.FilePath.equals(file.getAbsolutePath()))
                {
                    index = i;

                    for (String l : entry.ToDoLines)
                    {
                        ++todoCount;
                    }
                }
            }

            if (index == 0)
            {
                index = mEntryIndex;
                ++mEntryIndex;
            }

            // Remove beginning spaces
            line = line.trim();

            mEntries.get(index).ToDoCount = todoCount + 1;
            mEntries.get(index).FilePath = file.getAbsolutePath();
            mEntries.get(index).ToDoLines.add(line);
        }
    }

    private void onFile(File file)
    {
    //    System.out.println("Discovered file: " + file.getAbsolutePath());
    }

    private void onDirectory(File directory)
    {
    //    System.out.println("Discovered directory: " + directory.getAbsolutePath());
    }
}
