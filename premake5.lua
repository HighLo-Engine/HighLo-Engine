include "./vendor/bin/premake/solution_items.lua"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"]   = "%{wks.location}/HighLo/vendor/glfw/include"
IncludeDir["GLAD"]   = "%{wks.location}/HighLo/vendor/GLAD/include"
IncludeDir["glm"]    = "%{wks.location}/HighLo/vendor/glm"
IncludeDir["spdlog"] = "%{wks.location}/HighLo/vendor/spdlog/include"

LibDir = {}

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
	group ""

	group "Engine"
		include "HighLo"
	group ""

	group "Games"
		include "Sandbox"
		include "LevelEditor"
	group ""
