package en.highlo.statistics.utils;

import java.io.*;
import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;
import java.util.Scanner;
import java.util.function.BiConsumer;
import java.util.function.BiFunction;
import java.util.function.Consumer;

public class RecursiveDirectoryIterator
{
    private String mRootPath;
    private String[] mExcludeDirs;
    private int mIteratedFiles = 0;

    private Consumer<File> mFileCallback = null;
    private Consumer<File> mDirectoryCallback = null;
    private BiFunction<File, String, String> mLineCallback = null;

    public RecursiveDirectoryIterator(String rootPath, String[] excludeDirs)
    {
        mRootPath = rootPath;
        mExcludeDirs = excludeDirs;

        // Iterate through all files and count all files, to be able to provide the count before any callbacks are executed
        List<File> list = new LinkedList<>();
        iterateRecursiveAndCountFiles(list, new File(rootPath), excludeDirs);
    }

    public void registerFileCallback(Consumer<File> func)
    {
        mFileCallback = func;
    }

    public void registerDirectoryCallback(Consumer<File> func)
    {
        mDirectoryCallback = func;
    }

    public void registerLineCallback(BiFunction<File, String, String> func)
    {
        mLineCallback = func;
    }

    public void iterate()
    {
        List<File> list = new LinkedList<>();
        iterateRecursive(list, new File(mRootPath), mExcludeDirs);
    }

    public int getIteratedFiles()
    {
        return mIteratedFiles;
    }

    private List<File> iterateRecursiveAndCountFiles(List<File> files, File dir, String[] excludeDirs)
    {
        if (!dir.isDirectory())
        {
            ++mIteratedFiles;

            files.add(dir);
            return files;
        }

        for (File file : dir.listFiles())
        {
            if (excludeDirs != null && excludeDirs.length > 0)
            {
                String formattedFileName = StringUtils.convertToForwardSlash(file.getAbsolutePath());
                for (int i = 0; i < excludeDirs.length; ++i)
                {
                    String formattedExcludeName = StringUtils.convertToForwardSlash(excludeDirs[i]);
                    if (formattedExcludeName.endsWith("/"))
                    {
                        formattedExcludeName = formattedExcludeName.substring(0, formattedExcludeName.length() - 1);
                    }

                    if (!formattedExcludeName.equals(formattedFileName))
                    {
                        iterateRecursiveAndCountFiles(files, file, excludeDirs);
                    }
                }
            }
            else
            {
                iterateRecursiveAndCountFiles(files, file, null);
            }
        }

        return files;
    }

    private List<File> iterateRecursive(List<File> files, File dir, String[] excludeDirs)
    {

        if (!dir.isDirectory())
        {
            if (mFileCallback != null)
            {
                mFileCallback.accept(dir);
            }

            ArrayList<String> newContent = new ArrayList<>();

            try
            {
                Scanner s = new Scanner(dir);
                while (s.hasNextLine())
                {
                    String line = s.nextLine();
                    if (mLineCallback != null)
                    {
                        String newLine = mLineCallback.apply(dir, line);
                        newContent.add(newLine);
                    }
                }

                ++mIteratedFiles;
                s.close();
            }
            catch (FileNotFoundException e)
            {
                e.printStackTrace();
            }

            // Write out the new content into the current file
            try
            {
                FileOutputStream out = new FileOutputStream(dir);
                BufferedWriter bw = new BufferedWriter(new OutputStreamWriter(out));

                for (String line : newContent)
                {
                    bw.write(line);
                    bw.newLine();
                }

                bw.close();
            }
            catch (IOException e)
            {
                e.printStackTrace();
            }

            files.add(dir);
            return files;
        }

        for (File file : dir.listFiles())
        {
            if (excludeDirs != null && excludeDirs.length > 0)
            {
                String formattedFileName = StringUtils.convertToForwardSlash(file.getAbsolutePath());
                for (int i = 0; i < excludeDirs.length; ++i)
                {
                    String formattedExcludeName = StringUtils.convertToForwardSlash(excludeDirs[i]);
                    if (formattedExcludeName.endsWith("/"))
                    {
                        formattedExcludeName = formattedExcludeName.substring(0, formattedExcludeName.length() - 1);
                    }

                    if (!formattedExcludeName.equals(formattedFileName))
                    {
                        if (file.isDirectory() && mDirectoryCallback != null)
                        {
                            mDirectoryCallback.accept(file);
                        }

                        iterateRecursive(files, file, excludeDirs);
                    }
                }
            }
            else
            {
                if (file.isDirectory() && mDirectoryCallback != null)
                {
                    mDirectoryCallback.accept(file);
                }

                iterateRecursive(files, file, null);
            }
        }

        return files;
    }
}
