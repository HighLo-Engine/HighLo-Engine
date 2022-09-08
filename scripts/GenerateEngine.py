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
    print('--version                           Show the current version of the build script')
    print('--help                              Show this help menu')
    print('--visual-studio={value}             Generate the engine for a specific visual studio version (valid values are 2019 and 2022 for now)')
    print('--generate-unit-tests               Generate the unit tests for the engine')
    print('--project-name={projectName}        Generate a C# project that interacts with the engines C# library and can be run in the editor and runtime.')
    exit(0)


generateUnitTests = ''
shouldGenerateUnitTests = Utils.GetCommandLineArgument(sys.argv[1:], '--generate-unit-tests')
if shouldGenerateUnitTests[0]:
    generateUnitTests = '--generate-unit-tests=True'

shouldGenerateProject = Utils.GetCommandLineArgument(sys.argv[1:], '--project-name')

print('Your detected System is: ' + platform.system())

# Change from Scripts directory to root

# Make sure we are in the root directory
if 'scripts' in os.getcwd():
    os.chdir('../')

if (not Vulkan.CheckVulkanSDK()):
    exit(1)
    
if (not Vulkan.CheckVulkanSDKDebugLibs()):
    exit(1)

# If the user runs the script for the first time, make sure all dependencies exist
if (not os.path.exists('HighLo/bin')):
    print('Running the installer for the first time, making sure all lfs data is up-to-date...')
    subprocess.call(["git", "lfs", "pull"])
    subprocess.call(["git", "submodule", "update", "--init", "--recursive"])

if shouldGenerateProject[0]:
    projectName = shouldGenerateProject[1]
    print('Generating project ' + projectName)
    os.chdir('HighLoEdit/Resources/Projects/')
    
    if not os.path.isdir(projectName):
        os.mkdir(projectName)
        os.chdir(projectName)
        os.mkdir('src')
        os.chdir('../')
    
    if (platform.system() == 'Windows'):
        subprocess.call(["../../../vendor/bin/premake/Windows/premake5.exe", "vs2022", '--project-name=' + projectName])
    elif (platform.system() == 'Linux'):
        subprocess.call(["../../../vendor/bin/premake/Linux/premake5", "gmake", '--project-name=' + projectName])
    elif (platform.system() == 'Darwin'):
        subprocess.call(["../../../vendor/bin/premake/MacOS/premake5", "xcode4", '--project-name=' + projectName])
    
    os.chdir('../../../')

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
    
    