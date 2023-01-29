VULKAN_SDK = os.getenv("VULKAN_SDK")

IncludeDir = {}
IncludeDir["GLFW"]                   = "%{wks.location}/HighLo/vendor/glfw/include"
IncludeDir["GLAD"]                   = "%{wks.location}/HighLo/vendor/GLAD/include"
IncludeDir["glm"]                    = "%{wks.location}/HighLo/vendor/glm"
IncludeDir["spdlog"]                 = "%{wks.location}/HighLo/vendor/spdlog/include"
IncludeDir["assimp"]                 = "%{wks.location}/HighLo/vendor/assimp/include"
IncludeDir["stb_image"]              = "%{wks.location}/HighLo/vendor/stb_image"
IncludeDir["stb_truetype"]           = "%{wks.location}/HighLo/vendor/stb_truetype"
IncludeDir["openssl"]                = "%{wks.location}/HighLo/vendor/openssl/include"
IncludeDir["msdf_atlas_gen"]         = "%{wks.location}/HighLo/vendor/msdf-atlas-gen/msdf-atlas-gen"
IncludeDir["msdfgen"]                = "%{wks.location}/HighLo/vendor/msdf-atlas-gen/msdfgen"
IncludeDir["rapidXML"]               = "%{wks.location}/HighLo/vendor/RapidXML"
IncludeDir["rapidJSON"]              = "%{wks.location}/HighLo/vendor/RapidJSON/include"
IncludeDir["yamlCpp"]                = "%{wks.location}/HighLo/vendor/yaml-cpp/include"
IncludeDir["IconFontCppHeaders"]     = "%{wks.location}/HighLo/vendor/IconFontCppHeaders"
IncludeDir["mono"]     			 	 = "%{wks.location}/HighLo/vendor/mono/include"
IncludeDir["gtest"]     			 = "%{wks.location}/HighLo/vendor/googletest/googletest/include"
IncludeDir["gmock"]     			 = "%{wks.location}/HighLo/vendor/googletest/googlemock/include"
IncludeDir["vulkan"]     			 = "%{VULKAN_SDK}/Include"
IncludeDir["vulkan_local"] 			 = "%{wks.location}/HighLo/vendor/VulkanSDK/Include"
IncludeDir["vulkan_memory_alloc"] 	 = "%{wks.location}/HighLo/vendor/VulkanMemoryAllocator"
IncludeDir["SPIRV_Cross"] 	 	 	 = "%{wks.location}/HighLo/vendor/SPIRV-Cross"
IncludeDir["shaderc_glslc"] 	 	 = "%{wks.location}/HighLo/vendor/shaderc/glslc"
IncludeDir["shaderc_util"] 	 	 	 = "%{wks.location}/HighLo/vendor/shaderc/libshaderc_util/include"

LibDir = {}
LibDir["assimp"]                     = "%{wks.location}/HighLo/vendor/assimp/lib/Debug/assimp-vc142-mtd.lib"
LibDir["yamlCpp_debug"]              = "%{wks.location}/HighLo/vendor/yaml-cpp/lib/Debug/yaml-cppd.lib"
LibDir["yamlCpp"]                    = "%{wks.location}/HighLo/vendor/yaml-cpp/lib/Release/yaml-cpp.lib"
LibDir["openssl_libssl"]             = "%{wks.location}/HighLo/vendor/openssl/lib/libssl.lib"
LibDir["openssl_libssl_static"]      = "%{wks.location}/HighLo/vendor/openssl/lib/libssl_static.lib"
LibDir["openssl_libcrypto"]          = "%{wks.location}/HighLo/vendor/openssl/lib/libcrypto.lib"
LibDir["openssl_libcrypto_static"]   = "%{wks.location}/HighLo/vendor/openssl/lib/libcrypto_static.lib"

LibDir["gtest_debug"]    			 = "%{wks.location}/HighLo/vendor/googletest/lib/Debug/gtestd.lib"
LibDir["gtest_main_debug"]   		 = "%{wks.location}/HighLo/vendor/googletest/lib/Debug/gtest_maind.lib"
LibDir["gmock_debug"]    			 = "%{wks.location}/HighLo/vendor/googletest/lib/Debug/gmockd.lib"
LibDir["gmock_main_debug"]   		 = "%{wks.location}/HighLo/vendor/googletest/lib/Debug/gmock_maind.lib"
LibDir["gtest_release"]    			 = "%{wks.location}/HighLo/vendor/googletest/lib/Release/gtest.lib"
LibDir["gtest_main_release"]   		 = "%{wks.location}/HighLo/vendor/googletest/lib/Release/gtest_main.lib"
LibDir["gmock_release"]    			 = "%{wks.location}/HighLo/vendor/googletest/lib/Release/gmock.lib"
LibDir["gmock_main_release"]   		 = "%{wks.location}/HighLo/vendor/googletest/lib/Release/gmock_main.lib"

LibDir["mono"]   		 			 = "%{wks.location}/HighLo/vendor/mono/lib/Debug/libmono-static-sgen.lib"
LibDir["mono_release"]   			 = "%{wks.location}/HighLo/vendor/mono/lib/Release/libmono-static-sgen.lib"

LibDir["vulkan_sdk_folder"]  		 = "%{VULKAN_SDK}/Lib"
LibDir["vulkan_sdk"]   		 		 = "%{LibDir.vulkan_sdk_folder}/vulkan-1.lib"
LibDir["vulkan_sdk_utils"]  		 = "%{LibDir.vulkan_sdk_folder}/VKLayer_utils.lib"

--[[
LibDir["SPIRV_Debug"] 		 		 = "%{LibDir.vulkan_sdk_folder}/SPIRVd.lib"
LibDir["SPIRV_Cross_Debug"] 		 = "%{LibDir.vulkan_sdk_folder}/spirv-cross-cored.lib"
LibDir["SPIRV_Cross_GLSL_Debug"] 	 = "%{LibDir.vulkan_sdk_folder}/spirv-cross-glsld.lib"
LibDir["SPIRV_Cross_util_Debug"] 	 = "%{LibDir.vulkan_sdk_folder}/spirv-cross-utild.lib"
LibDir["SPIRV_Cross_cpp_Debug"] 	 = "%{LibDir.vulkan_sdk_folder}/spirv-cross-cppd.lib"
LibDir["SPIRV_Cross_reflect_Debug"]  = "%{LibDir.vulkan_sdk_folder}/spirv-cross-reflectd.lib"
LibDir["SpvRemapper_Debug"] 		 = "%{LibDir.vulkan_sdk_folder}/SPVRemapperd.lib"
LibDir["glsl_Debug"] 		 		 = "%{LibDir.vulkan_sdk_folder}/glslangd.lib"

LibDir["SPIRV_Release"] 		 	 = "%{LibDir.vulkan_sdk_folder}/SPIRV.lib"
LibDir["SPIRV_Cross_Release"] 		 = "%{LibDir.vulkan_sdk_folder}/spirv-cross-core.lib"
LibDir["SPIRV_Cross_GLSL_Release"] 	 = "%{LibDir.vulkan_sdk_folder}/spirv-cross-glsl.lib"
LibDir["SPIRV_Cross_util_Release"] 	 = "%{LibDir.vulkan_sdk_folder}/spirv-cross-util.lib"
LibDir["SPIRV_Cross_cpp_Release"] 	 = "%{LibDir.vulkan_sdk_folder}/spirv-cross-cpp.lib"
LibDir["SPIRV_Cross_reflect_Release"]= "%{LibDir.vulkan_sdk_folder}/spirv-cross-reflect.lib"
LibDir["SpvRemapper_Release"] 	 	 = "%{LibDir.vulkan_sdk_folder}/SPVRemapper.lib"
LibDir["glsl_Release"] 	 			 = "%{LibDir.vulkan_sdk_folder}/glslang.lib"
--]]

LibDir["SPIRV_Tools_Debug"] 		 = "%{LibDir.vulkan_sdk_folder}/SPIRV-Toolsd.lib"

LibDir["SPIRV_Cross_Debug"]          = "%{wks.location}/HighLo/vendor/SPIRV-Cross/lib/spirv-cross-cored.lib"
LibDir["SPIRV_Cross_Release"]        = "%{wks.location}/HighLo/vendor/SPIRV-Cross/lib/spirv-cross-core.lib"
LibDir["SPIRV_Cross_GLSL_Debug"]     = "%{wks.location}/HighLo/vendor/SPIRV-Cross/lib/spirv-cross-glsld.lib"
LibDir["SPIRV_Cross_GLSL_Release"]   = "%{wks.location}/HighLo/vendor/SPIRV-Cross/lib/spirv-cross-glsl.lib"

LibDir["shaderc_debug"]              = "%{LibDir.vulkan_sdk_folder}/shaderc_sharedd.lib"
LibDir["shaderc_util_debug"]         = "%{LibDir.vulkan_sdk_folder}/shaderc_utild.lib"

LibDir["shaderc_release"]            = "%{LibDir.vulkan_sdk_folder}/shaderc_shared.lib"
LibDir["shaderc_util_release"]       = "%{LibDir.vulkan_sdk_folder}/shaderc_util.lib"

LibDir["dxc"]    					 = "%{LibDir.vulkan_sdk_folder}/dxcompiler.lib"

-- Windows
LibDir["WinSock"] = "Ws2_32.lib"
LibDir["WinMM"] = "Winmm.lib"
LibDir["WinVersion"] = "Version.lib"
LibDir["WinBCrypt"] = "Bcrypt.lib"