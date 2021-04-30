project "glm"
	kind "StaticLib"
	language "C++"
	cppdialect "C++11"
	staticruntime "on"
	
	files
		{
		"glm/**.hpp"
		}
		
	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		runtime "Release"
		optimize "on"
