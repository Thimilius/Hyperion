include "packages.lua"

project "hyperion"
	location "hyperion"
	
	language "C++"
	cppdialect "C++20"
	architecture "x86_64"
	kind "StaticLib"
	
	staticruntime "On"
	exceptionhandling "Off"
	rtti "Off"
	flags { "FatalCompileWarnings" }
	
	pchheader "hyppch.hpp"
	pchsource "%{prj.location}/source/hyppch.cpp"

	files {
		"%{prj.location}/**.hpp",
		"%{prj.location}/**.h",
		"%{prj.location}/**.c",
		"%{prj.location}/**.cpp"
	}
	excludes {
		"%{prj.location}/include/hyperion/platform/**",
		"%{prj.location}/source/platform/**",

		"%{prj.location}/include/hyperion/modules/bullet/**",
		"%{prj.location}/source/modules/bullet/**",

        "%{prj.location}/vendor/glad/source/glad_wgl.c"
	}

	includedirs {
		"%{prj.location}/include",
		
		"%{prj.location}/vendor/glad/include",
		"%{prj.location}/vendor/nameof/include",
		"%{prj.location}/vendor/vulkan/include",
	}
	includedirs { package_assimp_includedirs }
	includedirs { package_freetype_includedirs }
	includedirs { package_shaderc_includedirs }
	includedirs { package_rttr_includedirs }
	includedirs { package_stb_includedirs }
	includedirs { package_vulkan_memory_allocator_includedirs }
	includedirs { package_yaml_includedirs }

	filter "files:hyperion/vendor/**"
		flags { "NoPCH" }

	filter "options:physics=bullet"
		files { 
			"%{prj.location}/include/hyperion/modules/bullet/**",
			"%{prj.location}/source/modules/bullet/**"
		}
		includedirs { package_bullet_includedirs }

	filter "configurations:profile"
		files { "%{prj.location}/vendor/optick/source/**" }
		includedirs { "%{prj.location}/vendor/optick/include" }

	filter "system:windows"
		files {
			"%{prj.location}/include/hyperion/platform/windows/**.hpp",
			"%{prj.location}/source/platform/windows/**.cpp",

            "%{prj.location}/vendor/glad/source/glad_wgl.c"
		}
		
function linkhyperion(path)
	filter { }

	includedirs { path .. "hyperion/include" }
	includedirs { path .. "hyperion/vendor/nameof/include" }
	includedirs { path .. "hyperion/vendor/optick/include" }
	includedirs { package_rttr_includedirs }

	links { "hyperion" }
	
	filter "kind:ConsoleApp"
		defines { "HYP_CONSOLE" }

	filter "system:windows"
		libdirs { path .. "hyperion/vendor/vulkan/lib/windows" }
		links {
			"PowrProf",

			"opengl32",

			"vulkan-1",
		}
	
	filter { "system:windows", "configurations:debug" }
		libdirs { package_assimp_debug_libdirs }
		libdirs { package_freetype_debug_libdirs }
		libdirs { package_yaml_debug_libdirs }
		libdirs { package_rttr_debug_libdirs }
		libdirs { package_shaderc_debug_libdirs }
		links { package_assimp_debug_links }
		links { package_freetype_debug_links }
		links { package_yaml_debug_links }
		links { package_rttr_debug_links }
		links { package_shaderc_debug_links }
	filter { "system:windows", "configurations:profile or release" }
		libdirs { package_assimp_release_libdirs }
		libdirs { package_freetype_release_libdirs }
		libdirs { package_yaml_release_libdirs }
		libdirs { package_rttr_release_libdirs }
		libdirs { package_shaderc_release_libdirs }
		links { package_assimp_release_links }
		links { package_freetype_release_links }
		links { package_yaml_release_links }
		links { package_rttr_release_links }
		links { package_shaderc_release_links }

	filter { "system:windows", "configurations:debug", "options:physics=bullet" }
		libdirs { package_bullet_debug_libdirs }
		links { package_bullet_debug_links }
	filter { "system:windows", "configurations:profile or release", "options:physics=bullet" }
		libdirs { package_bullet_release_libdirs }
		links { package_bullet_release_links }
		
	filter { }
end
