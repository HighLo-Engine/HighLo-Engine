project "HighLo"
    kind "StaticLib"
    language "C++"
	cppdialect "C++17"
	staticruntime "off"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-obj/" .. outputdir .. "/%{prj.name}")

	pchheader "HighLoPch.h"
	pchsource "src/HighLoPch.cpp"

    files
    { 
        "src/**.h",
        "src/**.cpp",
		
		"%{IncludeDir.vulkan_memory_alloc}/**.h",
		"%{IncludeDir.vulkan_memory_alloc}/**.cpp",
    }

    includedirs
    {
		"src",
		"%{IncludeDir.spdlog}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.assimp}",
        "%{IncludeDir.stb_image}",
		"%{IncludeDir.openssl}",
		"%{IncludeDir.msdf_atlas_gen}",
		"%{IncludeDir.msdfgen}",
		"%{IncludeDir.rapidXML}",
		"%{IncludeDir.rapidJSON}",
		"%{IncludeDir.yamlCpp}",
		"%{IncludeDir.IconFontCppHeaders}",
		"%{IncludeDir.gtest}",
		"%{IncludeDir.gmock}",
		"%{IncludeDir.vulkan}",
		"%{IncludeDir.vulkan_local}",
    }

    links
    {
        "GLFW",
        "stbimage",
		"msdf-atlas-gen",
        "%{LibDir.assimp}",
		
		"%{LibDir.openssl_libssl}",
		"%{LibDir.openssl_libcrypto}",
		"%{LibDir.vulkan_sdk}",
		"%{LibDir.vulkan_sdk_utils}",

    }

    defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_NONE",
		"HIGHLO_LIBRARY_EXPORT",
		"HIGHLO_ENABLE_PROFILER",
	}

    filter "system:windows"
        systemversion "latest"
        disablewarnings { "5033", "4996", "4217", "4006" }

        defines
        {
            "HL_PLATFORM_WINDOWS",
			--"HIGHLO_API_VULKAN",
			"HIGHLO_API_OPENGL",
			--"HIGHLO_API_DX11",
			--"HIGHLO_API_DX12",
			"HIGHLO_API_GLFW"	
        }

		includedirs
		{
		    "%{IncludeDir.GLAD}",
		}
		
		links
		{
		    "GLAD",
			"opengl32.lib",
		}

	filter "system:macosx"
        systemversion "latest"
        disablewarnings { "5033", "4996" }
		
        defines
        {
            "HL_PLATFORM_MAC",
			"HIGHLO_API_OPENGL",
			"HIGHLO_API_GLFW"	
        }

	filter "system:linux"
		systemversion "latest"
        disablewarnings { "5033", "4996" }

		defines
        {
            "HL_PLATFORM_LINUX",
			"HIGHLO_API_OPENGL",
			"HIGHLO_API_GLFW"	
        }

    filter "configurations:Debug"
        symbols "On"

		defines
		{
			"HL_DEBUG"
		}
		
		links
		{
			"%{LibDir.gtest_debug}",
			"%{LibDir.gtest_main_debug}",
			"%{LibDir.gmock_debug}",
			"%{LibDir.gmock_main_debug}",
			
			"%{LibDir.shaderc_debug}",
			"%{LibDir.shaderc_util_debug}",
			"%{LibDir.glsl_Debug}",

			"%{LibDir.SPIRV_Debug}",
			"%{LibDir.SPIRV_Cross_Debug}",
			"%{LibDir.SPIRV_Cross_GLSL_Debug}",
			"%{LibDir.SPIRV_Tools_Debug}",

			"%{LibDir.SPIRV_Cross_util_Debug}",
			"%{LibDir.SPIRV_Cross_cpp_Debug}",
			"%{LibDir.SPIRV_Cross_reflect_Debug}",
			"%{LibDir.SpvRemapper_Debug}",
		}

    filter "configurations:Release"
        optimize "On"
		
		defines
		{
			"HL_RELEASE",
			"NDEBUG",
		}
		
		links
		{
			"%{LibDir.gtest_release}",
			"%{LibDir.gtest_main_release}",
			"%{LibDir.gmock_release}",
			"%{LibDir.gmock_main_release}",
			
			
			"%{LibDir.shaderc_release}",
			"%{LibDir.shaderc_util_release}",
			"%{LibDir.glsl_Release}",
			
			"%{LibDir.SPIRV_Release}",
			"%{LibDir.SPIRV_Cross_Release}",
			"%{LibDir.SPIRV_Cross_GLSL_Release}",
			
			"%{LibDir.SPIRV_Cross_util_Release}",
			"%{LibDir.SPIRV_Cross_cpp_Release}",
			"%{LibDir.SPIRV_Cross_reflect_Release}",
			"%{LibDir.SpvRemapper_Release}",
		}
		
		