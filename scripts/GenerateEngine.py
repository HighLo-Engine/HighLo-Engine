import sys
import os
import subprocess
import platform

# First import that checks for other required packages
import CheckPythonVersion

# Make sure everything we need is installed
CheckPythonVersion.ValidatePackages()

import Vulkan
import Utils

shouldShowVersionMenu = Utils.GetCommandLineArgument(sys.argv[1:], '--version')
if shouldShowVersionMenu[0]:
    print('HighLo Build script for HighLo v1.0.0')
    exit(0)

shouldShowHelpMenu = Utils.GetCommandLineArgument(sys.argv[1:], '--help')
if shouldShowHelpMenu[0]:
    print('Usage: python GenerateEngine.py [option] {optionValue}')
    print('Valid options:')
    print('--version                Show the current version of the build script')
    print('--help                   Show this help menu')
    print('--visual-studio={value}  Generate the engine for a specific visual studio version (valid values are 2019 and 2022 for now)')
    print('--generate-unit-tests    Generate the unit tests for the engine')
    exit(0)


generateUnitTests = ''
shouldGenerateUnitTests = Utils.GetCommandLineArgument(sys.argv[1:], '--generate-unit-tests')
if shouldGenerateUnitTests[0]:
    generateUnitTests = '--generate-unit-tests=True'

print('Your detected System is: ' + platform.system())

# Change from Scripts directory to root

# Make sure we are in the root directory
if 'scripts' in os.getcwd():
    os.chdir('../')

if (not Vulkan.CheckVulkanSDK()):
    exit(1)
    
if (not Vulkan.CheckVulkanSDKDebugLibs()):
    exit(1)

# TODO: This should only be executed if user did a fresh clone
# subprocess.call(["git", "lfs", "pull"])
# subprocess.call(["git", "submodule", "update", "--init", "--recursive"])

print("Running premake...")

if (platform.system() == 'Windows'):
    if len(sys.argv) > 1:
        visualStudioVersion = Utils.GetCommandLineArgument(sys.argv[1:], '--visual-studio')

        if not visualStudioVersion[0]:
            # use default 
            subprocess.call(["vendor/bin/premake/Windows/premake5.exe", "vs2022", generateUnitTests])
            exit(0)

        if visualStudioVersion[1] == '2022':
            subprocess.call(["vendor/bin/premake/Windows/premake5.exe", "vs2022", generateUnitTests])
        elif visualStudioVersion[1] == '2019':
            subprocess.call(["vendor/bin/premake/Windows/premake5.exe", "vs2019", generateUnitTests])
        else:
            print('Error: Unknown Visual Studio version: ', visualStudioVersion[1], ' - 2019 or 2022 are valid')
            exit(1)
    else:
        # use the current visual studio version
        subprocess.call(["vendor/bin/premake/Windows/premake5.exe", "vs2022", generateUnitTests])
elif (platform.system() == 'Linux'):
    subprocess.call(["chmod", "+x", "vendor/bin/premake/Linux/premake5"])
    subprocess.call(["vendor/bin/premake/Linux/premake5", "gmake", generateUnitTests])
elif (platform.system() == 'Darwin'):
    subprocess.call(["chmod", "+x", "vendor/bin/premake/MacOS/premake5"])
    subprocess.call(["vendor/bin/premake/MacOS/premake5", "xcode4", generateUnitTests])
    
