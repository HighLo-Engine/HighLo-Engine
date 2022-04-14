import os
import subprocess
import sys
from pathlib import Path

import Utils

from io import BytesIO
from urllib.request import urlopen
from zipfile import ZipFile

VULKAN_VERSION = '1.3.204.1'
VULKAN_SDK_VERSION = '1.3.204.1'
VULKAN_SDK = os.environ.get('VULKAN_SDK')
VULKAN_SDK_INSTALLER_URL = f'https://sdk.lunarg.com/sdk/download/{VULKAN_VERSION}/windows/VulkanSDK-{VULKAN_VERSION}-Installer.exe'
VULKAN_SDK_LOCAL_PATH = 'HighLo/vendor/VulkanSDK'
VULKAN_SDK_EXE_PATH = f'{VULKAN_SDK_LOCAL_PATH}/VulkanSDK.exe'
VULKAN_SDK_DEBUG_LIBS_URL = f'https://sdk.lunarg.com/sdk/download/{VULKAN_SDK_VERSION}/windows/VulkanSDK-{VULKAN_SDK_VERSION}-DebugLibs.zip'
VULKAN_SDK_DEBUG_LIBS_ZIP =  f'{VULKAN_SDK_LOCAL_PATH}/VulkanDebugLibs.zip'

def InstallVulkanSDK():
    Path(VULKAN_SDK_LOCAL_PATH).mkdir(parents=True, exist_ok=True)
    print('Downloading {} to {}'.format(VULKAN_SDK_INSTALLER_URL, VULKAN_SDK_EXE_PATH))
    Utils.DownloadFile(VULKAN_SDK_INSTALLER_URL, VULKAN_SDK_EXE_PATH)
    print("Done!")
    print("Running Vulkan SDK installer...")
    os.startfile(os.path.abspath(VULKAN_SDK_EXE_PATH))
    print("Please restart this script after the installation")

def InstallVulkanPrompt():
    print("Would you like to install the Vulkan SDK?")
    install = Utils.YesOrNo()
    if (install):
        InstallVulkanSDK()
    quit()

def CheckVulkanSDK():
    if (VULKAN_SDK is None):
        print("You don't have the Vulkan SDK installed!")
        InstallVulkanPrompt()
        return False
    elif (VULKAN_VERSION not in VULKAN_SDK):
        print(f"Located Vulkan SDK at {VULKAN_SDK}")
        print(f"You don't have the correct Vulkan SDK version! (required version is {VULKAN_VERSION})")
        InstallVulkanPrompt()
        return False
    
    print(f"Correct Vulkan SDK located at {VULKAN_SDK}")
    return True

def CheckVulkanSDKDebugLibs():
    shadercdLib = Path(f"{VULKAN_SDK_LOCAL_PATH}/Lib/shaderc_sharedd.lib")
    if (not shadercdLib.exists()):
        print(f"No Vulkan SDK debug libs found. (Checked {shadercdLib})")
        print(f"{Path(VULKAN_SDK_DEBUG_LIBS_ZIP)}")
        if (not Path(VULKAN_SDK_DEBUG_LIBS_ZIP).exists()):
            if (not Path(VULKAN_SDK_LOCAL_PATH).exists()):
                Path(VULKAN_SDK_LOCAL_PATH).mkdir(parents=True, exist_ok=True)
            Utils.DownloadFile(VULKAN_SDK_DEBUG_LIBS_URL, VULKAN_SDK_DEBUG_LIBS_ZIP)
        print("Extracting...")
        with ZipFile(VULKAN_SDK_DEBUG_LIBS_ZIP, 'r') as zip:
            zip.extractall(VULKAN_SDK_LOCAL_PATH)

        os.remove(VULKAN_SDK_DEBUG_LIBS_ZIP)

    return True
    
