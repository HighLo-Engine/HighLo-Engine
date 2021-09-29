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
	
	group "Dependencies"
		include "../HighLo/vendor/glfw"
		include "../HighLo/vendor/glm"
		include "../HighLo/vendor/assimp"
		include "../HighLo/vendor/spdlog"
		include "../HighLo/vendor/openssl"
		include "../HighLo/vendor/HighLo-Unit"
		include "../HighLo/vendor/stb_image"
		include "../HighLo/vendor/msdf-atlas-gen"
		include "../HighLo/vendor/RapidXML"
		include "../HighLo/vendor/RapidJSON"
		include "../HighLo/vendor/yaml-cpp"
		
		filter "system:windows"
			include "../HighLo/vendor/GLAD"
	group ""
	
	group "tests"
		include "BinaryTreeTest"
		include "BinarySearchTreeTest"
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

