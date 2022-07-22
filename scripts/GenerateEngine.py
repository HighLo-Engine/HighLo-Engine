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

shouldShowVersionMenu = Utils.GetCommandLineArgument(sys.argv[1:], ('-v', '--version'), 'v', ['version'])
if (shouldShowVersionMenu):
    print('HighLo Build script for HighLo v1.0.0')
    exit(0)

shouldShowHelpMenu = Utils.GetCommandLineArgument(sys.argv[1:], ('-h', '--help'), 'h', ['help'])
if (shouldShowHelpMenu):
    print('Usage: python GenerateEngine.py [option] {optionValue}')
    print('Valid options:')
    print('-v         --version                Show the current version of the build script')
    print('-h         --help                   Show this help menu')
    print('-s {value} --visual-studio {value}  Generate the engine for a specific visual studio version (valid values are 2019 and 2022 for now)')
    exit(0)


generateUnitTests = ''
shouldGenerateUnitTests = Utils.GetCommandLineArgument(sys.argv[1:], ('-u', '--generate-unit-tests'), 'u', ['generate-unit-tests'])
if shouldGenerateUnitTests:
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
        visualStudioVersion = Utils.GetCommandLineArgument(sys.argv[1:], ('-s', '--visual-studio'), 's:', ['visual-studio=', 'vs='])
        if not visualStudioVersion:
            visualStudioVersion = ['2022']
        
        if visualStudioVersion[0] == '2022':
            subprocess.call(["vendor/bin/premake/Windows/premake5.exe", "vs2022", generateUnitTests])
        elif visualStudioVersion[0] == '2019':
            subprocess.call(["vendor/bin/premake/Windows/premake5.exe", "vs2019", generateUnitTests])
        else:
            print('Error: Unknown Visual Studio version: ', visualStudioVersion[0], ' - 2019 or 2022 are valid')
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
    
