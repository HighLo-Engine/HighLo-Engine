package en.highlo.statistics.copyrightUpdater;

import en.highlo.statistics.utils.RecursiveDirectoryIterator;

import java.io.File;

public class CopyRightUpdater
{
    private RecursiveDirectoryIterator mIterator;
    private String mCopyRightHeader;

    public CopyRightUpdater(String rootPath)
    {
        mIterator = new RecursiveDirectoryIterator(rootPath, null);
    }

    public CopyRightUpdater(String rootPath, String[] excludeDirs)
    {
        mIterator = new RecursiveDirectoryIterator(rootPath, excludeDirs);
    }

    public void iterateAndUpdate()
    {
        mIterator.registerLineCallback(this::onLine);
        mIterator.iterate();
    }

    public void setCopyRightHeader(String line)
    {
        mCopyRightHeader = line;
    }

    private String onLine(File file, String line)
    {
        if (line.contains("// Copyright (c)"))
        {
            // We found our copyright line
            return mCopyRightHeader;
        }

        return line;
    }
}
