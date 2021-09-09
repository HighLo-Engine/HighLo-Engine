project "HighLo"
    kind "StaticLib"
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
        "%{IncludeDir.SPIRV_Cross}",
		"%{IncludeDir.shaderc}",
		"%{IncludeDir.openssl}",
		"%{IncludeDir.HighLo_Unit}"
    }

    links
    {
        "GLFW",
        "GLAD",
        "stbimage",
        "%{LibDir.assimp}",
        "opengl32.lib",
		
		"%{LibDir.shaderc}",
		"%{LibDir.shaderc_util}",
		"%{LibDir.SPIRV_Cross_c}",
		"%{LibDir.SPIRV_Cross_core}",
		"%{LibDir.SPIRV_Cross_cpp}",
		"%{LibDir.SPIRV_Cross_glsl}",
		"%{LibDir.SPIRV_Cross_hlsl}",
		"%{LibDir.SPIRV_Cross_msl}",
		"%{LibDir.SPIRV_Cross_reflect}",
		"%{LibDir.SPIRV_Cross_util}",
		"%{LibDir.glslang}",
		"%{LibDir.glslang_MachineIndependent}",
		"%{LibDir.glslang_SPIRV}",
		"%{LibDir.glslang_OGLCompiler}",
		"%{LibDir.glslang_OSDependent}",
		"%{LibDir.glslang_GenericCodeGen}",
		"%{LibDir.SPIRV_Tools}",
		"%{LibDir.SPIRV_Tools_opt}",
		"%{LibDir.openssl_libssl}",
		"%{LibDir.openssl_libcrypto}",
		"%{LibDir.HighLo_Unit}"
    }

    defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_NONE",
		"HIGHLO_LIBRARY_EXPORT",
		"HIGHLO_ENABLE_PROFILER",
		"HIGHLO_API_OPENGL",
		--"HIGHLO_API_DX11",
		"HIGHLO_API_GLFW"
	}

    filter "system:windows"
        systemversion "latest"

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
