include "../vendor/bin/premake/solution_items.lua"
include "../Dependencies.lua"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"	

workspace "Highlo-Tests"
    architecture "x64"
    configurations { "Release" }
	
	solution_items
	{
		".editorconfig"
	}

	flags
	{
		"MultiProcessorCompile"
	}
	
	group "tests"
		include "StringTest"
		include "EncryptionTest"
		include "ListTest"
		include "HashmapTest"
		include "VectorTest"
		include "StackTest"
		include "QueueTest"
		include "ECSTest"
		include "FileTest"
		include "FileSystemTest"
		include "FileSystemPathTest"
	group ""

