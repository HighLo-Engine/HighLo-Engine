// Copyright (c) 2021 Albert Slepak and Can Karka. All rights reserved.

//
// version history:
//     - 1.0 (2021-09-14) initial release
//

#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Core/DataTypes/Hashmap.h"
#include "Engine/Core/DataTypes/String.h"

namespace highlo
{
	class VirtualFileSystem
	{
	public:

		/**
		 *
		 * Initializes the Virtual File System.
		 * This function is being called automatically by the Engine.
		 *
		 */
		HLAPI static void Init();

		/**
		 *
		 * Destroys the Virtual File System.
		 * This function is being called automatically by the Engine.
		 *
		 */
		HLAPI static void Shutdown();

		/**
		 *
		 * Mounts a virtual folderName to a physical folder structure on the disk.
		 * Use this function to register your real paths to a fictional folderName.
		 *
		 * @param virtualPath The fictional folderName that should be replaced by the actual folder structure.
		 * @param physicalPath The actual folderStructure that should replace the fictional folder name when actually accessing files on the disk.
		 */
		HLAPI void Mount(const HLString &virtualPath, const HLString &physicalPath);

		/**
		 *
		 * Deregisters a complete virtual path with all it's real paths attached on.
		 *
		 * @param path The virtual path that should be removed.
		 */
		HLAPI void Unmount(const HLString &path);

		/**
		 *
		 * This function translates a virtual folder name to a real path.
		 * This function does not have to be called by the client programmer, it is automatically being called by the other functions of the virtual filesystem.
		 *
		 * @param path The virtula path that should be resolved.
		 * @param outPath The actual physical path that gets set inside this function if the path really exists on the disk.
		 * @return Returns true if the virtual Path has been translated successfully into a physical path.
		 */
		HLAPI bool ResolvePhysicalPath(HLString &path, HLString &outPath);

		/**
		 *
		 * This function takes a virtual path to a file and returns it's content.
		 *
		 * @param path The virtual path to a file on the disk.
		 * @param outSize The Size of a file, that gets set inside this function.
		 * @return Returns the Data read from the File or nullptr on failure.
		 */
		HLAPI Byte *ReadFile(const HLString &path, int64 *outSize);

		/**
		 *
		 * This function takes a virtual path to a text file and returns it's content.
		 *
		 * @param path The virtual path to a file on the disk.
		 * @return Returns the Data read from the File or an empty String on failure.
		 */
		HLAPI HLString ReadTextFile(const HLString &path);

		/**
		 *
		 * Finds the Filename in a path and returns it without the extension.
		 *
		 * @param path The virtual path to a file on the disk.
		 * @return Returns the file name of a path without the extension.
		 */
		HLAPI HLString GetFileNameFromPath(const HLString &path);

		/**
		 *
		 * Finds the file extension in a path and returns it withoit the name.
		 *
		 * @param path The virtual path to a file on the disk.
		 * @return Returns the file extension of any file name.
		 */
		HLAPI HLString GetFileExtension(const HLString &path);

		/**
		 *
		 * This function takes a virtual path to a file and stores the provided data in that file.
		 *
		 * @param path The virtual path to a file on the disk.
		 * @param buffer The data which should be stored inside the file.
		 * @param size The Size of the data that should be stored.
		 * @return Returns true if the Data has been written successfully to the disk.
		 */
		HLAPI bool WriteFile(const HLString &path, Byte *buffer, int64 size);

		/**
		 *
		 * This function takes a virtual path to a file and stores the provided String in that file.
		 *
		 * @param path The virtual path to a file on the disk.
		 * @param text The Text that should be stored in the file.
		 * @return Returns true if the Data has been written successfully to the disk.
		 */
		HLAPI bool WriteTextFile(const HLString &path, const HLString &text);

		/**
		 *
		 * This function takes a virtual path to a file and removes it from the disk.
		 *
		 * @param path The virtual path to a file on the disk.
		 * @return Returns true if the File has been removed from the disk successfully.
		 */
		HLAPI bool RemoveFile(const HLString &path);

		/**
		 *
		 * This function takes a virtual path to a file and returns the size of it.
		 *
		 * @param path The virtual path to a file on the disk.
		 * @return Returns the Size in bytes of the file on the disk.
		 */
		HLAPI int64 GetFileSize(const HLString &path);

		/**
		 *
		 * This function takes a virtual path to a file and checks if it exists.
		 *
		 * @param path The virtual path to a file on the disk.
		 * @return Returns true if the File exists on the disk.
		 */
		HLAPI bool FileExists(const HLString &path);

		/**
		 *
		 * This function takes a virtual path and checks if it exists.
		 *
		 * @param path The virtual path to a folder on the disk.
		 * @return Returns true if the Path exists on the disk.
		 */
		HLAPI bool PathExists(const HLString &path);

		/**
		 *
		 * This function takes a virtual path to a non existing folder and creates it.
		 *
		 * @param path The virtual path to a folder on the disk.
		 * @return Returns true if the folder has been created on the disk.
		 */
		HLAPI bool CreateFolder(const HLString &path);

		/**
		 *
		 * This function takes a virtual path to a folder and removes it from the disk.
		 *
		 * @param path The virtual path to a folder on the disk.
		 * @return Returns true if the folder has been removed from the disk.
		 */
		HLAPI bool RemoveFolder(const HLString &path);

		/**
		 *
		 * This function takes a virtual path to a file or folder and opens it inside a explorer window.
		 *
		 * @param path The virtual path to a file on the disk.
		 */
		HLAPI void OpenInExplorer(const HLString &path);

		/**
		 *
		 * This function takes a URL and opens it in a browser.
		 *
		 * @param url The URL to a server that should be openend.
		 */
		HLAPI void OpenInBrowser(const HLString &url);

		/**
		 *
		 * Returns the real physical path calculated by the given virtual path as the parameter.
		 *
		 * @param path The virtual path from which the physical path should be calculated.
		 * @return Returns the physical path of a given virtual path.
		 */
		HLAPI HLString GetAbsoluteFilePath(const HLString &path);

		/**
		 *
		 * Returns a Instance of the virtual Filesystem.
		 *
		 * @return Returns a Instance of the Virtual filesystem.
		 */
		HLAPI static VirtualFileSystem *Get();

	private:

		static VirtualFileSystem *s_Instance;
		HLHashmap<HLString, std::vector<HLString>> m_MountPoints;
	};
}