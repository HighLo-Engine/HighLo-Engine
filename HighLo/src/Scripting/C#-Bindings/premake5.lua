project "HighLo-CSharp"
	kind "SharedLib"
	language "C#"
	dotnetframework "4.7.2"

	targetdir ("%{wks.location}/HighLoEdit/Resources/Scripts")
	objdir ("%{wks.location}/HighLoEdit/Resources/Scripts/Intermediates")

	files 
	{
		"src/**.cs",
	}
	
	filter "configurations:Debug"
		optimize "Off"
		symbols "Default"

	filter "configurations:Release"
		optimize "On"
		symbols "Default"

	filter "configurations:Dist"
		optimize "Full"
		symbols "Off"
		
	