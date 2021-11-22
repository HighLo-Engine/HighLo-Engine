import os
import subprocess
import CheckPythonVersion
import Vulkan
import platform

print('Your detected System is: ' + platform.system())

# Make sure everything we need is installed
CheckPythonVersion.ValidatePackages()

# Change from Scripts directory to root
os.chdir('../')

if (not Vulkan.CheckVulkanSDK()):
    print("Vulkan SDK not installed.")
    
if (not Vulkan.CheckVulkanSDKDebugLibs()):
    print("Vulkan SDK debug libs not found.")

subprocess.call(["git", "lfs", "pull"])
subprocess.call(["git", "submodule", "update", "--init", "--recursive"])

print("Running premake...")

os.chdir('scripts/')

if (platform.system() == 'Windows'):
    subprocess.call(["GenerateEngine-Windows.bat"])
elif (platform.system() == 'Linux'):
    subprocess.call(["chmod", "+x", "GenerateEngine-Linux.sh"])
    subprocess.call(["GenerateEngine-Linux.sh"])
elif (platform.system() == 'Darwin'):
    subprocess.call(["chmod", "+x", "GenerateEngine-MacOS.sh"])
    subprocess.call(["GenerateEngine-MacOS.sh"])
    
