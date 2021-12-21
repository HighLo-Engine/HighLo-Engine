project "HighLoTest"
    kind "ConsoleApp"
    language "C++"
	cppdialect "C++17"
	staticruntime "off"
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
		"%{IncludeDir.IconFontCppHeaders}",
		"%{IncludeDir.gtest}",
		"%{IncludeDir.gmock}"
    }

    links
    {
        "HighLo",
    }
	
	postbuildcommands
	{
		("{COPY} %{wks.location}HighLo/vendor/openssl/lib/libcrypto-3-x64.dll %{wks.location}tests/bin/" .. outputdir .. "/HighLoTest/libcrypto-3-x64.dll*"),
		("{COPY} %{wks.location}HighLo/vendor/openssl/lib/libssl-3-x64.dll %{wks.location}tests/bin/" .. outputdir .. "/HighLoTest/libssl-3-x64.dll*"),
		("{COPY} %{wks.location}HighLo/assets/editorconfig.ini %{wks.location}tests/bin/" .. outputdir .. "/HighLoTest/editorconfig.ini*"),
	}

    filter "system:windows"
        systemversion "latest"
        disablewarnings { "5033", "4996", "4217", "4006" }

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

    filter "configurations:Debug-OpenGL"
        defines "HL_DEBUG"
        symbols "On"

		links
		{
			"%{LibDir.gtest_debug}",
			"%{LibDir.gtest_main_debug}",
			"%{LibDir.gmock_debug}",
			"%{LibDir.gmock_main_debug}",
		}
		
		postbuildcommands
		{
			'{COPY} "%{wks.location}HighLo/vendor/assimp/lib/Debug/assimp-vc142-mtd.dll" "%{cfg.targetdir}"',
			'{COPY} "%{wks.location}HighLo/vendor/VulkanSDK/Bin/shaderc_sharedd.dll" "%{cfg.targetdir}"'
		}

	filter "configurations:Debug-Vulkan"
        defines "HL_DEBUG"
        symbols "On"

		links
		{
			"%{LibDir.gtest_debug}",
			"%{LibDir.gtest_main_debug}",
			"%{LibDir.gmock_debug}",
			"%{LibDir.gmock_main_debug}",
		}
		
		postbuildcommands
		{
			'{COPY} "%{wks.location}HighLo/vendor/assimp/lib/Debug/assimp-vc142-mtd.dll" "%{cfg.targetdir}"',
			'{COPY} "%{wks.location}HighLo/vendor/VulkanSDK/Bin/shaderc_sharedd.dll" "%{cfg.targetdir}"'
		}

	filter "configurations:Debug-DX11"
        defines "HL_DEBUG"
        symbols "On"

		links
		{
			"%{LibDir.gtest_debug}",
			"%{LibDir.gtest_main_debug}",
			"%{LibDir.gmock_debug}",
			"%{LibDir.gmock_main_debug}",
		}
		
		postbuildcommands
		{
			'{COPY} "%{wks.location}HighLo/vendor/assimp/lib/Debug/assimp-vc142-mtd.dll" "%{cfg.targetdir}"',
			'{COPY} "%{wks.location}HighLo/vendor/VulkanSDK/Bin/shaderc_sharedd.dll" "%{cfg.targetdir}"'
		}

	filter "configurations:Debug-DX12"
        defines "HL_DEBUG"
        symbols "On"

		links
		{
			"%{LibDir.gtest_debug}",
			"%{LibDir.gtest_main_debug}",
			"%{LibDir.gmock_debug}",
			"%{LibDir.gmock_main_debug}",
		}
		
		postbuildcommands
		{
			'{COPY} "%{wks.location}HighLo/vendor/assimp/lib/Debug/assimp-vc142-mtd.dll" "%{cfg.targetdir}"',
			'{COPY} "%{wks.location}HighLo/vendor/VulkanSDK/Bin/shaderc_sharedd.dll" "%{cfg.targetdir}"'
		}

    filter "configurations:Release-OpenGL"
        defines "HL_RELEASE"
        optimize "On"

		links
		{
			"%{LibDir.gtest_release}",
			"%{LibDir.gtest_main_release}",
			"%{LibDir.gmock_release}",
			"%{LibDir.gmock_main_release}",
		}
		
		postbuildcommands
		{
			'{COPY} "%{wks.location}HighLo/vendor/assimp/lib/Release/assimp-vc142-mt.dll" "%{cfg.targetdir}"',
		}

	filter "configurations:Release-Vulkan"
        defines "HL_RELEASE"
        optimize "On"

		links
		{
			"%{LibDir.gtest_release}",
			"%{LibDir.gtest_main_release}",
			"%{LibDir.gmock_release}",
			"%{LibDir.gmock_main_release}",
		}
		
		postbuildcommands
		{
			'{COPY} "%{wks.location}HighLo/vendor/assimp/lib/Release/assimp-vc142-mt.dll" "%{cfg.targetdir}"',
		}

	filter "configurations:Release-DX11"
        defines "HL_RELEASE"
        optimize "On"

		links
		{
			"%{LibDir.gtest_release}",
			"%{LibDir.gtest_main_release}",
			"%{LibDir.gmock_release}",
			"%{LibDir.gmock_main_release}",
		}
		
		postbuildcommands
		{
			'{COPY} "%{wks.location}HighLo/vendor/assimp/lib/Release/assimp-vc142-mt.dll" "%{cfg.targetdir}"',
		}

	filter "configurations:Release-DX12"
        defines "HL_RELEASE"
        optimize "On"

		links
		{
			"%{LibDir.gtest_release}",
			"%{LibDir.gtest_main_release}",
			"%{LibDir.gmock_release}",
			"%{LibDir.gmock_main_release}",
		}
		
		postbuildcommands
		{
			'{COPY} "%{wks.location}HighLo/vendor/assimp/lib/Release/assimp-vc142-mt.dll" "%{cfg.targetdir}"',
		}

	