import os
import subprocess
import CheckPythonVersion
import Vulkan

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
subprocess.call(["vendor/bin/premake/Windows/premake5.exe", "vs2019"])

