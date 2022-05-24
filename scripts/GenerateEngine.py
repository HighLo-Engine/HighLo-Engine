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

print('Your detected System is: ' + platform.system())

# Change from Scripts directory to root

# Make sure we are in the root directory
if 'scripts' in os.getcwd():
    os.chdir('../')

if (not Vulkan.CheckVulkanSDK()):
    print("Vulkan SDK not installed.")
    
if (not Vulkan.CheckVulkanSDKDebugLibs()):
    print("Vulkan SDK debug libs not found.")

# TODO: This should only be executed if user did a fresh clone
# subprocess.call(["git", "lfs", "pull"])
# subprocess.call(["git", "submodule", "update", "--init", "--recursive"])

print("Running premake...")

# Enter the scripts directory
os.chdir('scripts/')

if (platform.system() == 'Windows'):
    if len(sys.argv) > 1:
        visualStudioVersion = Utils.GetCommandLineArgument(sys.argv[1:], ('-v', '--visual-studio'), 'v:', ['visual-studio=', 'vs='])[0]
        if visualStudioVersion == '2022':
            subprocess.call(["GenerateEngine-Windows-vs2022.bat"])
        elif visualStudioVersion == '2019':
            subprocess.call(["GenerateEngine-Windows-vs2019.bat"])
        else:
            print('Error: Unknown Visual Studio version: ', visualStudioVersion, ' - 2019 or 2022 are valid')
    else:
        # use the current visual studio version
        subprocess.call(["GenerateEngine-Windows-vs2022.bat"])
elif (platform.system() == 'Linux'):
    subprocess.call(["chmod", "+x", "GenerateEngine-Linux.sh"])
    subprocess.call(["GenerateEngine-Linux.sh"])
elif (platform.system() == 'Darwin'):
    subprocess.call(["chmod", "+x", "GenerateEngine-MacOS.sh"])
    subprocess.call(["GenerateEngine-MacOS.sh"])
    
