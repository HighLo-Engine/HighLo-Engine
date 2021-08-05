project "SponzaSceneDemo"
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
		("{COPY} %{wks.location}HighLo/bin/" .. outputdir .. "/HighLo/HighLo.dll %{wks.location}demos/SponzaSceneDemo/bin/" .. outputdir .. "/SponzaSceneDemo/HighLo.dll*"),
		("{COPY} %{wks.location}HighLo/vendor/assimp/lib/Debug/assimp-vc142-mtd.dll %{wks.location}demos/SponzaSceneDemo/bin/" .. outputdir .. "/SponzaSceneDemo/assimp-vc142-mtd.dll*"),
		("{COPY} %{wks.location}HighLo/vendor/openssl/lib/libcrypto-3-x64.dll %{wks.location}demos/SponzaSceneDemo/bin/" .. outputdir .. "/SponzaSceneDemo/libcrypto-3-x64.dll*"),
		("{COPY} %{wks.location}HighLo/vendor/openssl/lib/libssl-3-x64.dll %{wks.location}demos/SponzaSceneDemo/bin/" .. outputdir .. "/SponzaSceneDemo/libssl-3-x64.dll*"),
		("{COPY} %{wks.location}HighLo/assets %{wks.location}demos/SponzaSceneDemo/bin/" .. outputdir .. "/SponzaSceneDemo/assets"),
		("{COPY} %{wks.location}HighLo/assets %{wks.location}demos/SponzaSceneDemo/assets") -- temporary because visual studio needs the assets in the working directory as well in order to find them
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
