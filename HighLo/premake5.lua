workspace "HighLo"
    architecture "x64"
    configurations { "Debug", "Release", "Dist" }
    startproject "Sandbox"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "HighLo/vendor/glfw/include"
IncludeDir["GLAD"] = "HighLo/vendor/GLAD/include"

group "Dependencies"
    include "HighLo/vendor/glfw"
	include "HighLo/vendor/GLAD"
group ""

project "HighLo"
    location "HighLo"
    kind "SharedLib"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-obj/" .. outputdir .. "/%{prj.name}")

    files
    { 
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs
    {
        "%{prj.name}/src",
        "%{prj.name}/vendor/spdlog/include",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.GLAD}"
    }

    links
    {
        "GLFW",
        "GLAD",
        "opengl32.lib"
    }

    defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_NONE"
	}

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"
        buildoptions "/MD"

        defines
        {
            "HL_BUILD",
            "HL_PLATFORM_WINDOWS"
        }

        postbuildcommands
        {
            ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
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

project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-obj/" .. outputdir .. "/%{prj.name}")

    files
    { 
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs
    {
        "HighLo/vendor/spdlog/include",
        "HighLo/src"
    }

    links
    {
        "HighLo"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"
        buildoptions "/MD"

        defines
        {
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
