include "./vendor/bin/premake/solution_items.lua"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"]       = "%{wks.location}/HighLo/vendor/glfw/include"
IncludeDir["GLAD"]       = "%{wks.location}/HighLo/vendor/GLAD/include"
IncludeDir["glm"]        = "%{wks.location}/HighLo/vendor/glm"
IncludeDir["spdlog"]     = "%{wks.location}/HighLo/vendor/spdlog/include"
IncludeDir["assimp"]     = "%{wks.location}/HighLo/vendor/assimp/include"
IncludeDir["imgui"]      = "%{wks.location}/HighLo/vendor/imgui"
IncludeDir["stb_image"]  = "%{wks.location}/HighLo/vendor/stb_image"

LibDir = {}
LibDir["assimp"]     = "%{wks.location}/HighLo/vendor/assimp/lib/Debug/assimp-vc141-mtd.lib"

workspace "HighLo"
    architecture "x64"
    configurations { "Debug", "Release", "Dist" }
    startproject "Sandbox"

	solution_items
	{
		".editorconfig"
	}

	group "Dependencies"
		include "HighLo/vendor/glfw"
		include "HighLo/vendor/GLAD"
		include "HighLo/vendor/glm"
		include "HighLo/vendor/assimp"
		include "HighLo/vendor/imgui"
		include "HighLo/vendor/spdlog"
		include "HighLo/vendor/stb_image"
	group ""

	group "Engine"
		include "HighLo"
	group ""

	group "Games"
		include "Sandbox"
		include "LevelEditor"
	group ""

