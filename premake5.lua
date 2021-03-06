include "./vendor/bin/premake/solution_items.lua"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "Dependencies.lua"

workspace "HighLo"
    architecture "x64"
    configurations { "Debug-OpenGL", "Release-OpenGL", "Debug-Vulkan", "Release-Vulkan", "Debug-DX11", "Release-DX11", "Debug-DX12", "Release-DX12" }
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
		include "HighLo/vendor/msdf-atlas-gen"
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

	group "tests"
		include "tests"
	group ""

	group "Tools"
		include "Sandbox"
		include "HighLoEdit"
	group ""
	
	group "Games"
	group ""

