include "../vendor/bin/premake/solution_items.lua"
include "../Dependencies.lua"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

workspace "HighLo-Tests"
    architecture "x64"
    configurations { "Debug", "Release", "Dist" }
    startproject "Sandbox"

	solution_items
	{
		".editorconfig"
	}

	flags
	{
		"MultiProcessorCompile"
	}


	include "StringTest"
	include "EncryptionTest"
	include "ListTest"
	include "HashmapTest"
	include "VectorTest"
	include "StackTest"
	include "QueueTest"

