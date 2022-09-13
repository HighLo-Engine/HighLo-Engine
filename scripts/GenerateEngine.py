import sys
import os
import subprocess
import platform
import shutil

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
    print('Usage: python GenerateEngine.py [option] or python GenerateEngine.py [option]="{value}"')
    print('Valid options:')
    print('--version                 Show the current version of the build script')
    print('--help                    Show this help menu')
    print('--visual-studio={value}   Generate the engine for a specific visual studio version (valid values are 2019 and 2022 for now)')
    print('--generate-unit-tests     Generate the unit tests for the engine')
    print('--project-name={value}    Determines the project name of the C# project in the visual studio solution.')
    print('--project-path={value}    Determines the path, where the source files should be and where the visual studio solution should be located. This option must be provided, if --project-name is also present.')
    exit(0)


generateUnitTests = ''
shouldGenerateUnitTests = Utils.GetCommandLineArgument(sys.argv[1:], '--generate-unit-tests')
if shouldGenerateUnitTests[0]:
    generateUnitTests = '--generate-unit-tests=True'

shouldGenerateProject = Utils.GetCommandLineArgument(sys.argv[1:], '--project-name')
projectPath = ''
if (shouldGenerateProject[0]):
    projectPath = Utils.GetCommandLineArgument(sys.argv[1:], '--project-path')

print('Your detected System is: ' + platform.system())

# Make sure we are in the root directory
if 'scripts' in os.getcwd():
    os.chdir('../')

# Store the root directory for later
rootDir = os.getcwd()

# Check if vulkan is installed
if (not Vulkan.CheckVulkanSDK()):
    exit(1)
    
# Check of the Vulkan Debug libs are installed
if (not Vulkan.CheckVulkanSDKDebugLibs()):
    exit(1)

# If the user runs the script for the first time, make sure all dependencies exist
if (not os.path.exists('HighLo/bin')):
    print('Running the installer for the first time, making sure all lfs data is up-to-date...')
    subprocess.call(["git", "lfs", "pull"])
    subprocess.call(["git", "submodule", "update", "--init", "--recursive"])

# This is a global way to generate a C# project. It can be used either directly by the user or by the engine itself.
# This generates not only a single C# project, but also links it correctly into the C# engine library,
# so that the client programmer can directly start using the engine's functions
if shouldGenerateProject[0]:
    projectName = shouldGenerateProject[1]
    print('Generating project ' + projectName + '...')
    os.chdir('HighLoEdit/Resources/Projects/')
     
    if not projectPath[0]:
        projectPath = ''
    else:
        projectPath = projectPath[1]
    
    if len(projectPath) == 0:
        print('Error: Project path can not be empty!')
        exit(1)
    
    # copy the premake file
    shutil.copy2('premake5.lua', projectPath + '/premake5.lua')
    
    # change the current directory to the project target but keep the current cwd as a backup
    os.chdir(projectPath)

    # create the src directory if it does not exist
    if not os.path.isdir('src'):
        os.mkdir('src')
    
    if (platform.system() == 'Windows'):
        subprocess.call([rootDir + "/vendor/bin/premake/Windows/premake5.exe", "vs2022", '--root-dir=' + rootDir, '--project-name=' + projectName, '--project-path=' + projectPath])
    elif (platform.system() == 'Linux'):
        subprocess.call([rootDir + "/vendor/bin/premake/Linux/premake5", "gmake", '--root-dir=' + rootDir, '--project-name=' + projectName, '--project-path=' + projectPath])
    elif (platform.system() == 'Darwin'):
        subprocess.call([rootDir + "/vendor/bin/premake/MacOS/premake5", "xcode4", '--root-dir=' + rootDir, '--project-name=' + projectName, '--project-path=' + projectPath])
    
    os.chdir(rootDir)
    print('Project ' + projectName + ' generated successfully.')

# Now generating the engine itself.
print("Generating HighLo...")

if (platform.system() == 'Windows'):
    if len(sys.argv) > 1:
        visualStudioVersion = Utils.GetCommandLineArgument(sys.argv[1:], '--visual-studio')

        if not visualStudioVersion[0]:
            # use default 
            subprocess.call(["vendor/bin/premake/Windows/premake5.exe", "vs2022", '--project-dir=' + projectPath, generateUnitTests])
            print('HighLo generated successfully.')
            exit(0)

        if visualStudioVersion[1] == '2022':
            subprocess.call(["vendor/bin/premake/Windows/premake5.exe", "vs2022", '--project-dir=' + projectPath, generateUnitTests])
        elif visualStudioVersion[1] == '2019':
            subprocess.call(["vendor/bin/premake/Windows/premake5.exe", "vs2019", '--project-dir=' + projectPath, generateUnitTests])
        else:
            print('Error: Unknown Visual Studio version: ', visualStudioVersion[1], ' - 2019 or 2022 are valid')
            exit(1)
    else:
        # use the current visual studio version
        subprocess.call(["vendor/bin/premake/Windows/premake5.exe", "vs2022", '--project-dir=' + projectPath, generateUnitTests])
elif (platform.system() == 'Linux'):
    subprocess.call(["chmod", "+x", "vendor/bin/premake/Linux/premake5"])
    subprocess.call(["vendor/bin/premake/Linux/premake5", "gmake", '--project-dir=' + projectPath, generateUnitTests])
elif (platform.system() == 'Darwin'):
    subprocess.call(["chmod", "+x", "vendor/bin/premake/MacOS/premake5"])
    subprocess.call(["vendor/bin/premake/MacOS/premake5", "xcode4", '--project-dir=' + projectPath, generateUnitTests])
    
print('HighLo generated successfully.')

    