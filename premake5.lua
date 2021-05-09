include "./vendor/bin/premake/solution_items.lua"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"]         = "%{wks.location}/HighLo/vendor/glfw/include"
IncludeDir["GLAD"]         = "%{wks.location}/HighLo/vendor/GLAD/include"
IncludeDir["glm"]          = "%{wks.location}/HighLo/vendor/glm"
IncludeDir["spdlog"]       = "%{wks.location}/HighLo/vendor/spdlog/include"
IncludeDir["assimp"]       = "%{wks.location}/HighLo/vendor/assimp/include"
IncludeDir["stb_image"]    = "%{wks.location}/HighLo/vendor/stb_image"
IncludeDir["SPIRV_Cross"]  = "%{wks.location}/HighLo/vendor/SPIRV-Cross"

LibDir = {}
LibDir["assimp"]                     = "%{wks.location}/HighLo/vendor/assimp/lib/Debug/assimp-vc142-mtd.lib"
LibDir["shaderc"]                    = "%{wks.location}/HighLovendor/shaderc/lib/%{cfg.buildcfg}/shaderc.lib"
LibDir["shaderc_util"]               = "%{wks.location}/HighLovendor/shaderc/lib/%{cfg.buildcfg}/shaderc_util.lib"
LibDir["glslang"]                    = "%{wks.location}/HighLovendor/glslang/lib/%{cfg.buildcfg}/glslang.lib"
LibDir["glslang_MachineIndependent"] = "%{wks.location}/HighLovendor/glslang/lib/%{cfg.buildcfg}/MachineIndependent.lib"
LibDir["glslang_SPIRV"]              = "%{wks.location}/HighLovendor/glslang/lib/%{cfg.buildcfg}/SPIRV.lib"
LibDir["glslang_OGLCompiler"]        = "%{wks.location}/HighLovendor/glslang/lib/%{cfg.buildcfg}/OGLCompiler.lib"
LibDir["glslang_OSDependent"]        = "%{wks.location}/HighLovendor/glslang/lib/%{cfg.buildcfg}/OSDependent.lib"
LibDir["glslang_GenericCodeGen"]     = "%{wks.location}/HighLovendor/glslang/lib/%{cfg.buildcfg}/GenericCodeGen.lib"
LibDir["SPIRV_Tools"]                = "%{wks.location}/HighLovendor/SPIRV-Tools/lib/%{cfg.buildcfg}/SPIRV-Tools.lib"
LibDir["SPIRV_Tools_opt"]            = "%{wks.location}/HighLovendor/SPIRV-Tools/lib/%{cfg.buildcfg}/SPIRV-Tools-opt.lib"

workspace "HighLo"
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

	group "Dependencies"
		include "HighLo/vendor/glfw"
		include "HighLo/vendor/GLAD"
		include "HighLo/vendor/glm"
		include "HighLo/vendor/assimp"
		include "HighLo/vendor/spdlog"
		include "HighLo/vendor/stb_image"
		--include "HighLo/vendor/SPIRV-Cross"
	group ""

	group "Engine"
		include "HighLo"
	group ""

	group "tests"
		include "tests"
	group ""

	group "Games"
		include "Sandbox"
		include "LevelEditor"
	group ""

