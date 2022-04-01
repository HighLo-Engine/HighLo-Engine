# Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

import os
import subprocess
import platform

if (platform.system() == 'Windows'):
    from benchmark_windows_utils import get_drives
else:
    from benchmark_unix_utils import get_drives

levelEditor = "HighLoEdit.exe"

def YesOrNo(prefix):
    while True:
        try:
            reply = str(input(prefix + ' [Y/N]: ')).lower().strip()
            if reply[:1] == 'y':
                return True
            if reply[:1] == 'n':
                return False
        except KeyboardInterrupt:
            return False

def getUserInput():
    try:
        reply = str(input('>>'))
        if len(reply) == 0:
            return None
        
        return reply
    except KeyboardInterrupt:
        return None

def findPath(filename):
    result = []
    
    # get_drives is now platform independent, on windows system it returns C:, D:, etc. and under unix systems it returns /
    drives = get_drives()
        
    for search_path in drives:
        for root, dir, files in os.walk(search_path):
            if filename in dir + files:
                result.append(os.path.abspath(os.path.join(root, filename)))
                
    return result
        

def pathsAreValid(paths):
    found = False

    for path in paths:
        if levelEditor in path:
            found = True

    # TODO: If we add support for more binaries in the future (e.g. Sandbox.exe or Runtime.exe)
    # we also have to validate them here

    return found

def getBinaryPaths():
    paths = []
    autoDetect = YesOrNo("Would you like to automatically detect the binary files of the engine?")
    if autoDetect:
        print("Searching for " + levelEditor + " on your pc...")
        paths = findPath(levelEditor)
    else:
    
        while True:
            print("Please provide a path to the file you want to benchmark:")
            path = getUserInput()
            if path:
                paths.append(path)
                print("You entered: " + path)
                
            doBreak = YesOrNo("Did you enter all paths you want to benchmark?")
            if doBreak:
                break
            
    # NEVER TRUST THE USER: because the user did enter .exe paths that are about to be executed we first have to check if the files are really part of the engine
    if pathsAreValid(paths):
        # now we have guaranteed input paths of the .exe files of the engine
        return paths
    else:
        print("You did not enter paths that are part of the engine. Stop.")
        quit()

if __name__ == "__main__":
    paths = getBinaryPaths()    
    print("these are your paths:")
            
    for path in paths:
        print(path)
            
            
    