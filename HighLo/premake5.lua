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
        "%{IncludeDir.stb_image}",
        --"%{IncludeDir.SPIRV_Cross}",
		--"%{IncludeDir.shaderc}"
    }

    links
    {
        "GLFW",
        "GLAD",
        --"SPIRV-Cross",
        "stbimage",
        "%{LibDir.assimp}",
        "opengl32.lib",
		
		--"%{LibDir.shaderc}",
		--"%{LibDir.shaderc_util}",
		-- "%{LibDir.glslang}",
		-- "%{LibDir.glslang_MachineIndependent}",
		-- "%{LibDir.glslang_SPIRV}",
		-- "%{LibDir.glslang_OGLCompiler}",
		-- "%{LibDir.glslang_OSDependent}",
		-- "%{LibDir.glslang_GenericCodeGen}",
		-- "%{LibDir.SPIRV_Tools}",
		-- "%{LibDir.SPIRV_Tools_opt}"
    }

    defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_NONE",
		"HIGHLO_USE_DLL",
		"HIGHLO_LIBRARY_EXPORT",
		"HIGHLO_ENABLE_PROFILER",
		"HIGHLO_API_OPENGL",
		--"HIGHLO_API_DX11",
		"HIGHLO_API_GLFW"
	}

    filter "system:windows"
        systemversion "latest"
        staticruntime "off"

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
