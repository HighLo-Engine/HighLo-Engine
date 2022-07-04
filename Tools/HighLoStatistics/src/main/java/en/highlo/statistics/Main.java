package en.highlo.statistics;

import en.highlo.statistics.copyrightUpdater.CopyRightUpdater;
import en.highlo.statistics.projectLineCounter.ProjectLineCounter;
import en.highlo.statistics.todoFinder.TODOEntry;
import en.highlo.statistics.todoFinder.TODOFinder;

import java.util.Calendar;
import java.util.List;

public class Main
{
    public static void main(String[] args)
    {
        // How it works:
        // basically this main class only parses the command line arguments and calls the specific classes, that were requested by the user
        // supported functions:
        // - a project line counter, that recursively iterates through all engine-related source directories and counts each line of each file and sums them up
        // - a todo finder, that is really useful for all developers, so that they can easily find a todo in any file without having to manually search for them
        // - a copyright updater that replaced the current copyright with a new one for every file
        // - more features are more than welcome by any other developer via pull-request or by a feature request! :)

        if (args.length < 2)
        {
            System.err.println("Invalid Argument count! Required to have at least 2 arguments!");
            return;
        }

        String desiredMode = args[0];
        String desiredRootPath = args[1];
        String[] excludeDirs = new String[args.length - 2]; // -2 because the first attribute is reserved for the desired mode and the second for the desired root path
        int j = 0;
        for (int i = 2; i < args.length; ++i)
        {
            excludeDirs[j] = args[i];
            ++j;
        }

        System.out.println("Selected Mode: " + desiredMode);
        System.out.println("Selected Root Path: " + desiredRootPath);
        System.out.println();

        if (desiredMode.equals("todo") || desiredMode.equals("TODO"))
        {
            TODOFinder finder = new TODOFinder(desiredRootPath, excludeDirs);
            finder.iterateAndCount();

            List<TODOEntry> entries = finder.getAllEntries();
            for (TODOEntry entry : entries)
            {
                System.out.println(entry.ToDoCount + " todos found in file " + entry.FilePath);

                for (String line : entry.ToDoLines)
                {
                    System.out.println("  " + line);
                }
                System.out.println();
            }
        }
        else if (desiredMode.equals("copyright"))
        {
            CopyRightUpdater updater = new CopyRightUpdater(desiredRootPath, excludeDirs);
            int currentYear = Calendar.getInstance().get(Calendar.YEAR);

            updater.setCopyRightHeader("// Copyright (c) 2021-" + currentYear + " Can Karka and Albert Slepak. All rights reserved.");
            updater.iterateAndUpdate();
        }
        else if (desiredMode.equals("lineCounter") || desiredMode.equals("linecounter") || desiredMode.equals("LINECOUNTER") || desiredMode.equals("line") || desiredMode.equals("LINE"))
        {
            ProjectLineCounter counter = new ProjectLineCounter(desiredRootPath, excludeDirs);
            counter.iterateAndCount();

            int totalLineCount = counter.getTotalLineCount();
            int totalFileCount = counter.getTotalFileCount();
            int totalDirCount = counter.getTotalDirectoyCount();

            System.out.println("Total Lines: " + totalLineCount);
            System.out.println("Total Files: " + totalFileCount);
            System.out.println("Total Directories: " + totalDirCount);
        }
    }
}
