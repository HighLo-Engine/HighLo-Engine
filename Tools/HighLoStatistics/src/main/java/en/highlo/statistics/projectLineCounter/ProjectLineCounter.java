package en.highlo.statistics.projectLineCounter;

import en.highlo.statistics.utils.RecursiveDirectoryIterator;

public class ProjectLineCounter
{
    private RecursiveDirectoryIterator mIterator;
    private int mLineCount = 0;
    private int mFileCount = 0;
    private int mDirectoryCount = 0;

    public ProjectLineCounter(String rootPath)
    {
        mIterator = new RecursiveDirectoryIterator(rootPath, null);
    }

    public ProjectLineCounter(String rootPath, String[] excludeDirs)
    {
        mIterator = new RecursiveDirectoryIterator(rootPath, excludeDirs);
    }

    public void iterateAndCount()
    {
        // Register all needed callbacks
        mIterator.registerDirectoryCallback(this::onDirectory);
        mIterator.registerFileCallback(this::onFile);
        mIterator.registerLineCallback(this::onLine);

        mIterator.iterate();
    }

    public int getTotalLineCount()
    {
        return mLineCount;
    }

    public int getTotalFileCount()
    {
        return mFileCount;
    }

    public int getTotalDirectoyCount()
    {
        return mDirectoryCount;
    }

    private void onLine(String filePath, String line)
    {
        ++mLineCount;
    }

    private void onFile(String filePath)
    {
        ++mFileCount;
    }

    private void onDirectory(String directoryPath)
    {
        ++mDirectoryCount;
    }
}