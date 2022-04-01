# Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

import os
import subprocess
import platform

if (platform.system() == 'Windows'):
    from benchmark_windows_utils import get_drives
else:
    from benchmark_unix_utils import get_drives

levelEditor = "HighLoEdit.exe"

def findPathOfFolder(filename):
    result = []
    
    # get_drives is now platform independent, on windows system it returns C:, D:, etc. and under unix systems it returns /
    drives = get_drives()
        
    for search_path in drives:
        for root, dir, files in os.walk(search_path):
            if filename in dir + files:
                result.append(os.path.abspath(os.path.join(root, filename)))
                
    return result
        

if __name__ == "__main__":
    print("Searching for " + levelEditor + " on your pc...")
    paths = findPathOfFolder(levelEditor)
    
    print("found these paths:")
    for path in paths:
        print(path)
    
    