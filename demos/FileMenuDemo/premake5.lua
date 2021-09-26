project "FileMenuDemo"
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
		"%{IncludeDir.glm}",
		"%{IncludeDir.HighLo_Unit}"
    }

    links
    {
        "HighLo"
    }

	postbuildcommands
	{
		("{COPY} %{wks.location}HighLo/vendor/assimp/lib/Debug/assimp-vc142-mtd.dll %{wks.location}demos/%{prj.name}/bin/" .. outputdir .. "/%{prj.name}/assimp-vc142-mtd.dll*"),
		("{COPY} %{wks.location}HighLo/vendor/openssl/lib/libcrypto-3-x64.dll %{wks.location}demos/%{prj.name}/bin/" .. outputdir .. "/%{prj.name}/libcrypto-3-x64.dll*"),
		("{COPY} %{wks.location}HighLo/vendor/openssl/lib/libssl-3-x64.dll %{wks.location}demos/%{prj.name}/bin/" .. outputdir .. "/%{prj.name}/libssl-3-x64.dll*"),
		("{COPY} %{wks.location}HighLo/vendor/HighLo-Unit/lib/HighLo-Unit.dll %{wks.location}demos/%{prj.name}/bin/" .. outputdir .. "/%{prj.name}/HighLo-Unit.dll*"),
		("{COPY} %{wks.location}HighLo/assets %{wks.location}demos/%{prj.name}/bin/" .. outputdir .. "/%{prj.name}/assets"),
		("{COPY} %{wks.location}HighLo/assets %{wks.location}demos/%{prj.name}/assets") -- temporary because visual studio needs the assets in the working directory as well in order to find them
	}

    filter "system:windows"
        systemversion "latest"
        disablewarnings { "5033", "4996" }

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

