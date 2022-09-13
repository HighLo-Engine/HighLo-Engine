newoption {
	trigger = "root-dir",
	description = "",
	default = "",
	value = "True"
}

newoption {
	trigger = "project-name",
	description = "Determines the project name of the visual studio solution.",
	default = "SandboxProject",
	value = "True"
}

newoption {
	trigger = "project-path",
	description = "Determines the path, where the source files should be and where the visual studio solution should be located.",
	default = "",
	value = "True"
}

ProjectName = _OPTIONS['project-name']
ProjectPath = _OPTIONS['project-path']
RootDir = _OPTIONS['root-dir']

include (RootDir .. "/vendor/bin/premake/solution_items.lua")

workspace (ProjectName)
	targetdir "build"
	startproject (ProjectName)
	
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

	project (ProjectName)
		location (ProjectPath .. '/' .. ProjectName)
		kind "SharedLib"
		language "C#"
		dotnetframework "4.7.2"
		
		targetdir (ProjectPath .. "/build")
		objdir (ProjectPath .. "/build/Intermediates")
				
		files
		{
			"%{ProjectPath}/%{ProjectName}/src/**.cs",
		}
		
		includedirs
		{
			"%{ProjectPath}/%{ProjectName}/src",
		}
		
		links
		{
			(RootDir .. "/HighLoEdit/Resources/Scripts/HighLo-C#.dll")
		}
		
		