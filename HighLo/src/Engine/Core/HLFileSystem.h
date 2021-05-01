#pragma once

#include "Engine/Core/DataTypes/HLString.h"

namespace highlo
{
	class FileSystem
	{
	public:

		/**
		 *
		 * This function takes a path to a file and checks if it exists.
		 *
		 * @param path The physical path to a file on the disk.
		 * @return Returns true if the File exists on the disk.
		 */
		static bool FileExists(const HLString &path);

		/**
		 *
		 * This function takes a path and checks if it exists.
		 *
		 * @param path The physical path to a folder on the disk.
		 * @return Returns true if the Path exists on the disk.
		 */
		static bool PathExists(const HLString &path);

		/**
		 *
		 * This function takes a path to a file and removes it from the disk.
		 *
		 * @param path The physical path to a file on the disk.
		 * @return Returns true if the File has been removed from the disk.
		 */
		static bool RemoveFile(const HLString &path);

		/**
		 *
		 * This function takes a path to a file and returns the size of it.
		 *
		 * @param path The physical path to a file on the disk.
		 * @return Returns the Size of the file on the disk.
		 */
		static int64 GetFileSize(const HLString &path);

		/**
		 *
		 * This function takes a path to a non existing folder and creates it.
		 *
		 * @param path The physical path to a folder on the disk.
		 * @return Returns true if the folder has been created on the disk.
		 */
		static bool CreateFolder(const HLString &path);

		/**
		 *
		 * This function takes a path to a folder and removes it from the disk.
		 *
		 * @param path The physical path to a folder on the disk.
		 * @return Returns true if the folder has been removed from the disk.
		 */
		static bool RemoveFolder(const HLString &path);

		/**
		 *
		 * This function takes a path to a file and returns it's content.
		 *
		 * @param path The physical path to a file on the disk.
		 * @param outSize The Size of a file, that gets set inside this function.
		 * @return Returns the Data read from the File or nullptr on failure.
		 */
		static Byte *ReadFile(const HLString &path, int64 *outSize);

		/**
		 *
		 * This function takes a path to a text file and returns it's content.
		 *
		 * @param path The physical path to a file on the disk.
		 * @return Returns the Data read from the File or an empty String on failure.
		 */
		static HLString ReadTextFile(const HLString &path);

		/**
		 *
		 * This function takes a path to a file and stores the provided data in that file.
		 *
		 * @param path The physical path to a file on the disk.
		 * @param buffer The data which should be stored inside the file.
		 * @param size The Size of the data that should be stored.
		 * @return Returns true if the Data has been written successfully to the disk.
		 */
		static bool WriteFile(const HLString &path, Byte *buffer, int64 size);

		/**
		 *
		 * This function takes a path to a file and stores the provided String in that file.
		 *
		 * @param path The physical path to a file on the disk.
		 * @param text The Text that should be stored in the file.
		 * @return Returns true if the Data has been written successfully to the disk.
		 */
		static bool WriteTextFile(const HLString &path, const HLString &text);

		/**
		 *
		 * This function takes a path to a file or folder and opens it inside a explorer window.
		 *
		 * @param path The physical path to a file on the disk.
		 */
		static void OpenInExplorer(const HLString &path);

		/**
		 *
		 * This function takes a URL and opens it in a browser.
		 *
		 * @param url The URL to a server that should be openend.
		 */
		static void OpenInBrowser(const HLString &url);
	};
}