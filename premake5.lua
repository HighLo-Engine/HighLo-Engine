include "./vendor/bin/premake/solution_items.lua"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "Dependencies.lua"

newoption {
	trigger = "generate-unit-tests",
	description = "Generate the unit tests",
	default = "False",
	value = "True"
}

newoption {
	trigger = "project-dir",
	description = "Describes the path to the script folder. The script folder is either provided by the engine when the user interacts with the editor or by the user, when he uses the GenerateEngine.py script",
	default = "False",
	value = "True"
}

projectDir = _OPTIONS['project-dir']

workspace "HighLo"
    architecture "x64"
    configurations { "Debug-OpenGL", "Release-OpenGL", "Debug-Vulkan", "Release-Vulkan", "Debug-DX11", "Release-DX11", "Debug-DX12", "Release-DX12", "Debug-Metal", "Release-Metal" }
    startproject "HighLoEdit"

	solution_items
	{
		".editorconfig"
	}

	flags
	{
		"MultiProcessorCompile"
	}

	group "Dependencies"
		include "HighLo/vendor/glfw"
		include "HighLo/vendor/glm"
		include "HighLo/vendor/assimp"
		include "HighLo/vendor/spdlog"
		include "HighLo/vendor/openssl"
		include "HighLo/vendor/stb_image"
		include "HighLo/vendor/stb_truetype"
		include "HighLo/vendor/RapidXML"
		include "HighLo/vendor/RapidJSON"
		include "HighLo/vendor/IconFontCppHeaders"
		include "HighLo/vendor/googletest"
		include "HighLo/vendor/shaderc"
		include "HighLo/vendor/yaml-cpp"
		
		filter "system:windows"
			include "HighLo/vendor/GLAD"
	group ""

	group "Engine"
		include "HighLo"
		include "HighLo/src/Scripting/C#-Bindings"
	group ""

	group "Demos"
		include "Demos"
	group ""

	-- check if the command line arguments contain the generateUnitTests flag
	if _OPTIONS['generate-unit-tests'] == "True" then
		print('generating the unit tests...')
		group "tests"
			include "Tests"
		group ""
	end
	
	group "Tools"
		include "Sandbox"
		include "HighLoEdit"
	group ""
	
	group "Games"
	group ""

