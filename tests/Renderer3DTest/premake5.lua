project "Renderer3DTest"
    kind "ConsoleApp"
    language "C++"
	cppdialect "C++17"
	staticruntime "on"
	entrypoint "mainCRTStartup"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-obj/" .. outputdir .. "/%{prj.name}")

    files
    { 
        "src/**.h",
        "src/**.cpp"
    }

    includedirs
    {
		"src",
		"%{wks.location}/HighLo/src",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.glm}"
    }

    links
    {
        "HighLo"
    }

	defines
	{
		"HL_USE_DLL"
	}

	postbuildcommands
	{
		("{COPY} %{wks.location}HighLo/bin/" .. outputdir .. "/HighLo/HighLo.dll %{wks.location}tests/Renderer3DTest/bin/" .. outputdir .. "/Renderer3DTest/HighLo.dll*"),
		("{COPY} %{wks.location}HighLo/vendor/assimp/lib/Debug/assimp-vc141-mtd.dll %{wks.location}tests/Renderer3DTest/bin/" .. outputdir .. "/Renderer3DTest/assimp-vc141-mtd.dll*"),
		("{COPY} %{wks.location}HighLo/assets %{wks.location}tests/Renderer3DTest/bin/" .. outputdir .. "/Renderer3DTest/assets"),
		("{COPY} %{wks.location}tests/Renderer3DTest/assets %{wks.location}tests/Renderer3DTest/bin/" .. outputdir .. "/Renderer3DTest/assets"),
	}

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"
        staticruntime "off"

        disablewarnings { "5033" }

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