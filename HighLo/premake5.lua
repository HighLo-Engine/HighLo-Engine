project "HighLo"
    kind "SharedLib"
    language "C++"
	cppdialect "C++17"
	staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-obj/" .. outputdir .. "/%{prj.name}")

	pchheader "HighLoPch.h"
	pchsource "src/HighLoPch.cpp"

    files
    { 
        "src/**.h",
        "src/**.cpp"
    }

    includedirs
    {
		"src",
		"%{IncludeDir.spdlog}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.GLAD}",
        "%{IncludeDir.assimp}",
        "%{IncludeDir.imgui}"
    }

    links
    {
        "GLFW",
        "GLAD",
        "ImGui",
        "%{LibDir.assimp}",
        "opengl32.lib"
    }

    defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_NONE",
		"HL_USE_DLL",
		"HL_LIBRARY_EXPORT",
		"HL_ENABLE_PROFILER",
		"HIGHLO_API_OPENGL",
		--"HIGHLO_API_GLFW"
	}

    filter "system:windows"
        systemversion "latest"
        buildoptions "/MD"

        disablewarnings { "5033" }

        defines
        {
            "HL_BUILD",
            "HL_PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        defines "HL_DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "HL_RELEASE"
        optimize "On"

    filter "configurations:Dist"
        defines "HL_DIST"
        optimize "On"
