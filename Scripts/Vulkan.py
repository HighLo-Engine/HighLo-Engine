import os
import subprocess
import platform
from pathlib import Path

import Utils

from io import BytesIO
from urllib.request import urlopen
from zipfile import ZipFile

VULKAN_VERSION = '1.3.204.1'
VULKAN_SDK = os.environ.get('VULKAN_SDK')
VULKAN_SDK_INSTALLER_URL = f'https://sdk.lunarg.com/sdk/download/{VULKAN_VERSION}/windows/VulkanSDK-{VULKAN_VERSION}-Installer.exe'
VULKAN_SDK_INSTALLER_LINUX_URL = f'https://sdk.lunarg.com/sdk/download/{VULKAN_VERSION}/linux/vulkansdk-linux-x86_64-{VULKAN_VERSION}.tar.gz'

VULKAN_SDK_LOCAL_PATH = 'HighLo/vendor/VulkanSDK'
VULKAN_SDK_EXE_PATH = f'{VULKAN_SDK_LOCAL_PATH}/VulkanSDK.exe'
VULKAN_SDK_TAR_NAME = f'vulkansdk-linux-x86_64-{VULKAN_VERSION}.tar.gz'
VULKAN_SDK_TAR_PATH = f'{VULKAN_SDK_LOCAL_PATH}/{VULKAN_SDK_TAR_NAME}'

def InstallVulkanSDK_Windows(isFirstInstall):
    Path(VULKAN_SDK_LOCAL_PATH).mkdir(parents=True, exist_ok=True)
    print('Downloading {} to {}'.format(VULKAN_SDK_INSTALLER_URL, VULKAN_SDK_EXE_PATH))

    Utils.DownloadFile(VULKAN_SDK_INSTALLER_URL, VULKAN_SDK_EXE_PATH)
    print("Done!")
    
    print("Running Vulkan SDK installer...")
    os.startfile(os.path.abspath(VULKAN_SDK_EXE_PATH))
    print("Please restart this script after the installation")

def InstallVulkanSDK_Linux(isFirstInstall):
    # First clean up the local path
    Path(VULKAN_SDK_LOCAL_PATH).mkdir(parents=True, exist_ok=True)
    currentDir = os.getcwd()
    os.chdir(Path(VULKAN_SDK_LOCAL_PATH))
    subprocess.call(f'rm -rf *', shell=True)
    os.chdir(currentDir)

    print('Downloading {} to {}'.format(VULKAN_SDK_INSTALLER_LINUX_URL, VULKAN_SDK_TAR_PATH))
    Utils.DownloadFile(VULKAN_SDK_INSTALLER_LINUX_URL, VULKAN_SDK_TAR_PATH)
    print('Done!')

    print('Unpacking tar archive...')
    currentDir = os.getcwd()
    os.chdir(Path(VULKAN_SDK_LOCAL_PATH))
    subprocess.call(f"tar -zxf {VULKAN_SDK_TAR_NAME}", shell=True)
    print('Done!')

    print('Removing downloaded file...')
    os.remove(Path(VULKAN_SDK_TAR_NAME)) # remove the downloaded file
    print('Done!')

    # Now move everything from x86_64 folder to current folder
    subprocess.call(f'mv {VULKAN_VERSION}/setup-env.sh .', shell=True)
    subprocess.call(f'mv {VULKAN_VERSION}/x86_64/* .', shell=True)
    subprocess.call(f'mv include Include', shell=True)
    subprocess.call(f'rm -rf {VULKAN_VERSION}', shell=True)

    os.chdir(currentDir)
    print(f'Please copy the contents of {VULKAN_SDK_LOCAL_PATH}/setup-env.sh into your .profile file in your home directory, but replace the VULKAN_SDK path with {VULKAN_SDK_LOCAL_PATH} (VERY important to use a absolute path!)')
    print('After that re-login into your account and restart this script.')

def InstallVulkanSDK(isFirstInstall):
    if (platform.system() == 'Windows'):
        InstallVulkanSDK_Windows(isFirstInstall)
    elif (platform.system() == 'Linux'):
        InstallVulkanSDK_Linux(isFirstInstall)
    else:
        print(f'The system {platform.system()} is not yet supported. Please open a new issue on github.')

def InstallVulkanPrompt():
    print("Would you like to install the Vulkan SDK?")
    return Utils.YesOrNo()

def CheckVulkanSDK(isFirstInstall):
    if (VULKAN_SDK is None):
        print("You don't have the Vulkan SDK installed!")
        if InstallVulkanPrompt():
            InstallVulkanSDK(isFirstInstall)
            exit(0)
        return False
    # TODO: This is not really a error, the user could decide to place the VulkanSDK at any location on his PC, without having the version as part of the path...
#    elif (VULKAN_VERSION not in VULKAN_SDK):
#        print(f"Located Vulkan SDK at {VULKAN_SDK}")
#        print(f"You don't have the correct Vulkan SDK version! (required version is {VULKAN_VERSION})")
#        if InstallVulkanPrompt():
#            InstallVulkanSDK(isFirstInstall)
#            exit(0)
#        return False
    
    print(f"Correct Vulkan SDK located at {VULKAN_SDK}")
    return True

def GetWindowsVulkanSDKDebugLibs():
    return Path(f"{VULKAN_SDK}/Lib/shaderc_sharedd.lib")

def GetLinuxVulkanSDKDebugLibs():
    return Path(f"{VULKAN_SDK}/lib/libshaderc_shared.so")

def CheckVulkanSDKDebugLibs():
    if (platform.system() == 'Windows'):
        shadercdLib = GetWindowsVulkanSDKDebugLibs()
    elif (platform.system() == 'Linux'):
        shadercdLib = GetLinuxVulkanSDKDebugLibs()
    else:
        print(f'The system {platform.system()} is not yet supported. Please open a new issue on github.')

    if (not shadercdLib.exists()):
        print(f"No Vulkan SDK debug libs found. (Checked {shadercdLib})")
        print("Please follow the documentation and install the Debug Libs of Vulkan as well. You have to tick the optional checkbox in the Vulkan installer. For more information check the documentation at https://docs.highlo-engine.com/latest/getting-started")
        return False

    print(f"Correct Vulkan SDK Debug Libs found at {shadercdLib}")
    return True
    
