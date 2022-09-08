ProjectName = "DemoProject"
RootDir = "../../../.."
local OutputDir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include (RootDir .. "/vendor/bin/premake/solution_items.lua")

workspace (ProjectName .. "")
	targetdir "build"
	startproject (ProjectName .. "")
	
	solution_items
	{
		(RootDir .. "/.editorconfig")
	}
	
	configurations
	{
		"Debug",
		"Release",
	}
	
	defines
	{
		"GLM_FORCE_DEPTH_ZERO_TO_ONE",
	}
	
	group "HighLo"
		project "HighLo-C#"
			location (RootDir .. "/HighLo/src/Scripting/C#-Bindings")
			kind "SharedLib"
			language "C#"
			dotnetframework "4.7.2"
			
			targetdir (RootDir .. "/HighLoEdit/Resources/Scripts")
			objdir (RootDir .. "/HighLoEdit/Resources/Scripts/Intermediates")
			
			files
			{
				"%{RootDir}/HighLo/src/Scripting/C#-Bindings/src/**.cs"
			}
	group ""

	project (ProjectName .. "")
		location ""
		kind "SharedLib"
		language "C#"
		dotnetframework "4.7.2"
		
		targetdir (RootDir .. "/HighLoEdit/Resources/Scripts")
		objdir (RootDir .. "/HighLoEdit/Resources/Scripts/Intermediates")
		
		files
		{
			"src/**.cs"
		}
		
		links
		{
			"HighLo-C#"
		}
		
		