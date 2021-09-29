project "BinaryTreeTest"
    kind "ConsoleApp"
    language "C++"
	cppdialect "C++17"
	staticruntime "on"
	entrypoint "mainCRTStartup"

    targetdir ("bin")
    objdir ("bin-obj")

    files
    { 
        "src/**.h",
        "src/**.cpp"
    }

    includedirs
    {
		"src",
		"%{wks.location}/../HighLo/src",
		"%{wks.location}/../HighLo/vendor/spdlog/include",
		"%{wks.location}/../HighLo/vendor/glm",
		"%{wks.location}/../HighLo/vendor/HighLo-Unit/include"
    }

    links
    {
		"GLFW",
        "stbimage",
		"msdf-atlas-gen",
	
        "%{wks.location}/../HighLo/bin/" .. outputdir .. "/HighLo/HighLo.lib",
		"%{wks.location}/../HighLo/vendor/HighLo-Unit/lib/HighLo-Unit.lib"
    }

	defines
	{
		"HIGHLO_UNIT_USE_DLL"
	}

	postbuildcommands
	{
		("{COPY} %{wks.location}/../HighLo/vendor/assimp/lib/Debug/assimp-vc142-mtd.dll %{wks.location}/%{prj.name}/bin/assimp-vc142-mtd.dll*"),
		("{COPY} %{wks.location}/../HighLo/vendor/openssl/lib/libcrypto-3-x64.dll %{wks.location}/%{prj.name}/bin/libcrypto-3-x64.dll*"),
		("{COPY} %{wks.location}/../HighLo/vendor/openssl/lib/libssl-3-x64.dll %{wks.location}/%{prj.name}/bin/libssl-3-x64.dll*"),
		("{COPY} %{wks.location}/../HighLo/vendor/HighLo-Unit/lib/HighLo-Unit.dll %{wks.location}/%{prj.name}/bin/HighLo-Unit.dll*")
	}

    filter "system:windows"
        systemversion "latest"
        disablewarnings { "5033" }

        defines
        {
            "HL_PLATFORM_WINDOWS"
        }

		links
		{
			"GLAD",
			"opengl32.lib"
		}

    filter "configurations:Debug"
        defines "HL_DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "HL_RELEASE"
        optimize "On"

