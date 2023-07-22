project "Sandbox"
    kind "ConsoleApp"
    language "C++"
	cppdialect "C++17"
	staticruntime "off"
	entrypoint "mainCRTStartup"

	dependson
	{
		"HighLo",
		"HighLo-C#",
	}

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
		"%{IncludeDir.HighLo_Unit}",
		"%{IncludeDir.IconFontCppHeaders}",
    }

    links
    {
        "HighLo"
    }

	postbuildcommands
	{
		("{COPY} %{wks.location}HighLo/assets %{wks.location}Sandbox/assets")
	}

    filter "system:windows"
        systemversion "latest"
        disablewarnings { "5033", "4996" }

        defines
        {
            "HL_PLATFORM_WINDOWS"
        }

		postbuildcommands
		{
			("{COPY} %{wks.location}HighLo/vendor/openssl/lib/libcrypto-3-x64.dll %{wks.location}Sandbox/bin/" .. outputdir .. "/Sandbox/libcrypto-3-x64.dll*"),
			("{COPY} %{wks.location}HighLo/vendor/openssl/lib/libssl-3-x64.dll %{wks.location}Sandbox/bin/" .. outputdir .. "/Sandbox/libssl-3-x64.dll*"),	
		}

	filter "system:linux"
        systemversion "latest"

        defines
        {
            "HL_PLATFORM_LINUX"
        }

	filter "system:macosx"
        systemversion "latest"
		
        defines
        {
            "HL_PLATFORM_MAC"
        }

    filter "configurations:Debug-OpenGL"
        defines "HL_DEBUG"
        symbols "On"

	filter { "configurations:Debug-OpenGL", "system:windows" }

		postbuildcommands
		{
			'{COPY} "%{wks.location}HighLo/vendor/assimp/lib/Debug/assimp-vc142-mtd.dll" "%{cfg.targetdir}"',
			'{COPY} "%{VULKAN_SDK}/Bin/shaderc_sharedd.dll" "%{cfg.targetdir}"'
		}

    filter "configurations:Debug-Vulkan"
        defines "HL_DEBUG"
        symbols "On"

	filter { "configurations:Debug-Vulkan", "system:windows" }

		postbuildcommands
		{
			'{COPY} "%{wks.location}HighLo/vendor/assimp/lib/Debug/assimp-vc142-mtd.dll" "%{cfg.targetdir}"',
			'{COPY} "%{VULKAN_SDK}/Bin/shaderc_sharedd.dll" "%{cfg.targetdir}"'
		}

    filter "configurations:Debug-DX11"
        defines "HL_DEBUG"
        symbols "On"

	filter { "configurations:Debug-DX11", "system:windows" }

		postbuildcommands
		{
			'{COPY} "%{wks.location}HighLo/vendor/assimp/lib/Debug/assimp-vc142-mtd.dll" "%{cfg.targetdir}"',
			'{COPY} "%{VULKAN_SDK}/Bin/shaderc_sharedd.dll" "%{cfg.targetdir}"'
		}

    filter "configurations:Debug-DX12"
        defines "HL_DEBUG"
        symbols "On"

	filter { "configurations:Debug-DX12", "system:windows" }
	
		postbuildcommands
		{
			'{COPY} "%{wks.location}HighLo/vendor/assimp/lib/Debug/assimp-vc142-mtd.dll" "%{cfg.targetdir}"',
			'{COPY} "%{VULKAN_SDK}/Bin/shaderc_sharedd.dll" "%{cfg.targetdir}"'
		}

    filter "configurations:Debug-Metal"
        defines "HL_DEBUG"
        symbols "On"

	filter { "configurations:Debug-Metal", "system:windows" }

		postbuildcommands
		{
			'{COPY} "%{wks.location}HighLo/vendor/assimp/lib/Debug/assimp-vc142-mtd.dll" "%{cfg.targetdir}"',
			'{COPY} "%{VULKAN_SDK}/Bin/shaderc_sharedd.dll" "%{cfg.targetdir}"'
		}

    filter "configurations:Release-OpenGL"
        defines "HL_RELEASE"
        optimize "On"

	filter { "configurations:Release-OpenGL", "system:windows" }

		postbuildcommands
		{
			'{COPY} "%{wks.location}HighLo/vendor/assimp/lib/Release/assimp-vc142-mt.dll" "%{cfg.targetdir}"',
		}
		
    filter "configurations:Release-Vulkan"
        defines "HL_RELEASE"
        optimize "On"

	filter { "configurations:Release-Vulkan", "system:windows" }

		postbuildcommands
		{
			'{COPY} "%{wks.location}HighLo/vendor/assimp/lib/Release/assimp-vc142-mt.dll" "%{cfg.targetdir}"',
		}

    filter "configurations:Release-DX11"
        defines "HL_RELEASE"
        optimize "On"

	filter { "configurations:Release-DX11", "system:windows" }

		postbuildcommands
		{
			'{COPY} "%{wks.location}HighLo/vendor/assimp/lib/Release/assimp-vc142-mt.dll" "%{cfg.targetdir}"',
		}

    filter "configurations:Release-DX12"
        defines "HL_RELEASE"
        optimize "On"

	filter { "configurations:Release-DX12", "system:windows" }

		postbuildcommands
		{
			'{COPY} "%{wks.location}HighLo/vendor/assimp/lib/Release/assimp-vc142-mt.dll" "%{cfg.targetdir}"',
		}

	filter "configurations:Release-Metal"
        defines "HL_RELEASE"
        optimize "On"

	filter { "configurations:Release-Metal", "system:windows" }

		postbuildcommands
		{
			'{COPY} "%{wks.location}HighLo/vendor/assimp/lib/Release/assimp-vc142-mt.dll" "%{cfg.targetdir}"',
		}
		
		