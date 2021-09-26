include "./vendor/bin/premake/solution_items.lua"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "Dependencies.lua"

workspace "HighLo"
    architecture "x64"
    configurations { "Debug", "Release" }
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
		include "HighLo/vendor/HighLo-Unit"
		include "HighLo/vendor/stb_image"
		include "HighLo/vendor/msdf-atlas-gen"
		include "HighLo/vendor/RapidXML"
		
		filter "system:windows"
			include "HighLo/vendor/GLAD"
	group ""

	group "Engine"
		include "HighLo"
	group ""

	group "Demos"
		include "demos"
	group ""

	group "Games"
		include "Sandbox"
		include "HighLoEdit"
	group ""

