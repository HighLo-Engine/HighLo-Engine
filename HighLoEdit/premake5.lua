project "HighLoEdit"
    kind "ConsoleApp"
    language "C++"
	cppdialect "C++17"
	staticruntime "on"
	entrypoint "mainCRTStartup"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    debugdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-obj/" .. outputdir .. "/%{prj.name}")

    files
    { 
        "src/**.h",
        "src/**.cpp"
    }

    includedirs
    {
		"src",
		"../HighLo/src",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.IconFontCppHeaders}"
    }

    links
    {
        "HighLo"
    }
	
	postbuildcommands
	{
		("{COPY} %{wks.location}HighLo/vendor/assimp/lib/Debug/assimp-vc142-mtd.dll %{wks.location}HighLoEdit/bin/" .. outputdir .. "/HighLoEdit/assimp-vc142-mtd.dll*"),
		("{COPY} %{wks.location}HighLo/vendor/openssl/lib/libcrypto-3-x64.dll %{wks.location}HighLoEdit/bin/" .. outputdir .. "/HighLoEdit/libcrypto-3-x64.dll*"),
		("{COPY} %{wks.location}HighLo/vendor/openssl/lib/libssl-3-x64.dll %{wks.location}HighLoEdit/bin/" .. outputdir .. "/HighLoEdit/libssl-3-x64.dll*"),
		("{COPY} %{wks.location}HighLo/vendor/HighLo-Unit/lib/HighLo-Unit.dll %{wks.location}HighLoEdit/bin/" .. outputdir .. "/HighLoEdit/HighLo-Unit.dll*"),
		("{COPY} %{wks.location}HighLo/assets/editorconfig.ini %{wks.location}HighLoEdit/bin/" .. outputdir .. "/HighLoEdit/editorconfig.ini*"),
		("{COPY} %{wks.location}HighLo/assets %{wks.location}HighLoEdit/bin/" .. outputdir .. "/HighLoEdit/assets"),
		("{COPY} %{wks.location}HighLoEdit/assets %{wks.location}HighLoEdit/bin/" .. outputdir .. "/HighLoEdit/assets")
	}

    filter "system:windows"
        systemversion "latest"
        disablewarnings { "5033", "4996" }

        defines
        {
            "HL_PLATFORM_WINDOWS"
        }

	filter "system:linux"
		systemversion "latest"
		
		defines
		{
			"HL_PLATFORM_LINUX"
		}

	filter "system:macos"
		systemversion "latest"
		
		defines
		{
			"HL_PLATFORM_MACOS"
		}

    filter "configurations:Debug"
        defines "HL_DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "HL_RELEASE"
        optimize "On"

