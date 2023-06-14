-- create empty cache files
shaderFileHandle = io.open("assets/shaders.registry", "w")
shaderFileHandle:close()
assetFileHandle = io.open("assets/assets.registry", "w")
assetFileHandle:close()


project "HighLoEdit"
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
		"%{IncludeDir.IconFontCppHeaders}",
		"%{IncludeDir.vulkan}",
    }

    links
    {
        "HighLo"
    }
	
	if projectDir ~= "False" then
		debugargs
		{
			"--project-path",
			"%{projectDir}",
		}
	end
	
	postbuildcommands
	{
		("{COPY} %{wks.location}HighLo/vendor/openssl/lib/libcrypto-3-x64.dll %{wks.location}HighLoEdit/bin/" .. outputdir .. "/HighLoEdit/libcrypto-3-x64.dll*"),
		("{COPY} %{wks.location}HighLo/vendor/openssl/lib/libssl-3-x64.dll %{wks.location}HighLoEdit/bin/" .. outputdir .. "/HighLoEdit/libssl-3-x64.dll*"),
		
		-- copy all contents of the asset folder but the cache.registry file (if you copy the file everytime you build, the current cache registry will be overriden, which will cause a false recompilation of all shaders)
		("{COPY} %{wks.location}HighLo/assets/editorconfig.ini %{wks.location}HighLoEdit"),
		("{COPY} %{wks.location}HighLo/assets/cache %{wks.location}HighLoEdit/assets/cache"),
		("{COPY} %{wks.location}HighLo/assets/fonts %{wks.location}HighLoEdit/assets/fonts"),
		("{COPY} %{wks.location}HighLo/assets/models %{wks.location}HighLoEdit/assets/models"),
		("{COPY} %{wks.location}HighLo/assets/Resources %{wks.location}HighLoEdit/assets/Resources"),
		("{COPY} %{wks.location}HighLo/assets/shaders %{wks.location}HighLoEdit/assets/shaders"),
		("{COPY} %{wks.location}HighLo/assets/textures %{wks.location}HighLoEdit/assets/textures")
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

    filter "configurations:Debug-OpenGL"
        defines "HL_DEBUG"
        symbols "On"
		
		postbuildcommands
		{
			'{COPY} "%{wks.location}HighLo/vendor/assimp/lib/Debug/assimp-vc142-mtd.dll" "%{cfg.targetdir}"',
			'{COPY} "%{VULKAN_SDK}/Bin/shaderc_sharedd.dll" "%{cfg.targetdir}"'
		}

	filter "configurations:Debug-Vulkan"
        defines "HL_DEBUG"
        symbols "On"
		
		postbuildcommands
		{
			'{COPY} "%{wks.location}HighLo/vendor/assimp/lib/Debug/assimp-vc142-mtd.dll" "%{cfg.targetdir}"',
			'{COPY} "%{VULKAN_SDK}/Bin/shaderc_sharedd.dll" "%{cfg.targetdir}"'
		}

	filter "configurations:Debug-DX11"
        defines "HL_DEBUG"
        symbols "On"
		
		postbuildcommands
		{
			'{COPY} "%{wks.location}HighLo/vendor/assimp/lib/Debug/assimp-vc142-mtd.dll" "%{cfg.targetdir}"',
			'{COPY} "%{VULKAN_SDK}/Bin/shaderc_sharedd.dll" "%{cfg.targetdir}"'
		}

	filter "configurations:Debug-DX12"
        defines "HL_DEBUG"
        symbols "On"
		
		postbuildcommands
		{
			'{COPY} "%{wks.location}HighLo/vendor/assimp/lib/Debug/assimp-vc142-mtd.dll" "%{cfg.targetdir}"',
			'{COPY} "%{VULKAN_SDK}/Bin/shaderc_sharedd.dll" "%{cfg.targetdir}"'
		}

	filter "configurations:Debug-Metal"
        defines "HL_DEBUG"
        symbols "On"
		
		postbuildcommands
		{
			'{COPY} "%{wks.location}HighLo/vendor/assimp/lib/Debug/assimp-vc142-mtd.dll" "%{cfg.targetdir}"',
			'{COPY} "%{VULKAN_SDK}/Bin/shaderc_sharedd.dll" "%{cfg.targetdir}"'
		}

    filter "configurations:Release-OpenGL"
        defines "HL_RELEASE"
        optimize "On"

		postbuildcommands
		{
			'{COPY} "%{wks.location}HighLo/vendor/assimp/lib/Debug/assimp-vc142-mtd.dll" "%{cfg.targetdir}"',
			'{COPY} "%{wks.location}HighLo/vendor/assimp/lib/Release/assimp-vc142-mt.dll" "%{cfg.targetdir}"',
		}
		
	filter "configurations:Release-Vulkan"
        defines "HL_RELEASE"
        optimize "On"

		postbuildcommands
		{
			'{COPY} "%{wks.location}HighLo/vendor/assimp/lib/Debug/assimp-vc142-mtd.dll" "%{cfg.targetdir}"',
			'{COPY} "%{wks.location}HighLo/vendor/assimp/lib/Release/assimp-vc142-mt.dll" "%{cfg.targetdir}"',
		}

	filter "configurations:Release-DX11"
        defines "HL_RELEASE"
        optimize "On"

		postbuildcommands
		{
			'{COPY} "%{wks.location}HighLo/vendor/assimp/lib/Debug/assimp-vc142-mtd.dll" "%{cfg.targetdir}"',
			'{COPY} "%{wks.location}HighLo/vendor/assimp/lib/Release/assimp-vc142-mt.dll" "%{cfg.targetdir}"',
		}

	filter "configurations:Release-DX12"
        defines "HL_RELEASE"
        optimize "On"

		postbuildcommands
		{
			'{COPY} "%{wks.location}HighLo/vendor/assimp/lib/Debug/assimp-vc142-mtd.dll" "%{cfg.targetdir}"',
			'{COPY} "%{wks.location}HighLo/vendor/assimp/lib/Release/assimp-vc142-mt.dll" "%{cfg.targetdir}"',
		}

	filter "configurations:Release-Metal"
        defines "HL_RELEASE"
        optimize "On"

		postbuildcommands
		{
			'{COPY} "%{wks.location}HighLo/vendor/assimp/lib/Debug/assimp-vc142-mtd.dll" "%{cfg.targetdir}"',
			'{COPY} "%{wks.location}HighLo/vendor/assimp/lib/Release/assimp-vc142-mt.dll" "%{cfg.targetdir}"',
		}