# Copyright (c) 2021-2022 Can Karka and Albert Slepak. All rights reserved.

import string
from ctypes import windll

# this function returns all drives connected to the windows system
def get_drives():
    drives = []
    bitmask = windll.kernel32.GetLogicalDrives()
    for letter in string.ascii_uppercase:
        if bitmask & 1:
            drives.append(letter)
        bitmask >>= 1

    # append :\\ to each drive path
    for i in range(len(drives)):
        drives[i] = drives[i] + ':\\'

    return drives