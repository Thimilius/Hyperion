output_directory_format = "%{cfg.system}-%{cfg.architecture}"
audio_backend_option = "audio"
physics_backend_option = "physics"

include "packages.lua"

workspace "hyperion"
	startproject "hyperion-sandbox"

	targetdir ("build/%{cfg.buildcfg}/bin/" .. output_directory_format)
	objdir ("build/%{cfg.buildcfg}/obj/" .. output_directory_format)
	debugdir "run_tree/"

	configurations { "debug", "profile", "release" }

	newoption {
		trigger = audio_backend_option,
		value = "API",
		description = "Specifies the audio backend",
		allowed = {
			{ "none", "None" }
		}
	}
	if not _OPTIONS[audio_backend_option] then
		_OPTIONS[audio_backend_option] = "none"
	end

	newoption {
		trigger = physics_backend_option,
		value = "API",
		description = "Specifies the physics backend",
		allowed = {
			{ "none", "None" },
			{ "bullet", "Bullet" }
		}
	}
	if not _OPTIONS[physics_backend_option] then
		_OPTIONS[physics_backend_option] = "bullet"
	end

	filter "configurations:debug"	
		defines { "HYP_DEBUG", "HYP_ENABLE_ASSERTS", "HYP_BREAK_ON_ASSERT" }
		runtime "Debug"
		symbols "On"
	filter "configurations:profile"	
		defines { "HYP_PROFILE", "HYP_ENABLE_ASSERTS", "HYP_BREAK_ON_ASSERT" }
		runtime "Release"
		optimize "On"
	filter "configurations:release"
		defines { "HYP_RELEASE" }
		runtime "Release"
		optimize "On"
	
	filter "system:windows"
		defines { "HYP_PLATFORM_WINDOWS", "_CRT_SECURE_NO_WARNINGS", "_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS" }
		systemversion "latest"
		linkoptions { "/IGNORE:4099" }

	filter "options:audio=none"
		defines { "HYP_AUDIO_NONE" }

	filter "options:physics=none"
		defines { "HYP_PHYSICS_NONE" }
	filter "options:physics=bullet"
		defines { "HYP_PHYSICS_BULLET" }

project "hyperion"
	location "projects/hyperion"
	
	language "C++"
	cppdialect "C++17"
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
		"%{prj.location}/vendor/vulkan/include",
	}
	includedirs { package_assimp_includedirs }
	includedirs { package_fmt_includedirs }
	includedirs { package_freetype_includedirs }
	includedirs { package_nlohmann_includedirs }
	includedirs { package_stb_includedirs }
	includedirs { package_yaml_includedirs }
			
	filter "files:projects/hyperion/vendor/**"
		flags { "NoPCH" }

	filter "options:physics=bullet"
		files { 
			"%{prj.location}/include/hyperion/modules/bullet/**",
			"%{prj.location}/source/modules/bullet/**"
		}
		includedirs { package_bullet_includedirs }

	filter "system:windows"
		files {
			"%{prj.location}/include/hyperion/platform/windows/**.hpp",
			"%{prj.location}/source/platform/windows/**.cpp",

            "%{prj.location}/vendor/glad/source/glad_wgl.c"
		}
		
function linkhyperion()
	filter { }

	includedirs { "projects/hyperion/include" }
	includedirs { package_fmt_includedirs }

	links { "hyperion" }
	
	filter "kind:ConsoleApp"
		defines { "HYP_CONSOLE" }

	filter "system:windows"
		libdirs { "projects/hyperion/vendor/vulkan/lib/windows" }
		links { "opengl32", "PowrProf", "vulkan-1" }
	
	filter { "system:windows", "configurations:debug" }
		libdirs { package_assimp_debug_libdirs }
		libdirs { package_fmt_debug_libdirs }
		libdirs { package_freetype_debug_libdirs }
		libdirs { package_yaml_debug_libdirs }
		links { package_assimp_debug_links }
		links { package_fmt_debug_links }
		links { package_freetype_debug_links }
		links { package_yaml_debug_links }
	filter { "system:windows", "configurations:profile or release" }
		libdirs { package_assimp_release_libdirs }
		libdirs { package_fmt_release_libdirs }
		libdirs { package_freetype_release_libdirs }
		libdirs { package_yaml_release_libdirs }
		links { package_assimp_release_links }
		links { package_fmt_release_links }
		links { package_freetype_release_links }
		links { package_yaml_release_links }

	filter { "system:windows", "configurations:debug", "options:physics=bullet" }
		libdirs { package_bullet_debug_libdirs }
		links { package_bullet_debug_links }
	filter { "system:windows", "configurations:profile or release", "options:physics=bullet" }
		libdirs { package_bullet_release_libdirs }
		links { package_bullet_release_links }
		
	filter { }

end

project "hyperion-sandbox"
	location "projects/hyperion-sandbox"
	
	language "C++"
	cppdialect "C++17"
	architecture "x86_64"
	kind "WindowedApp"
	
	staticruntime "On"
	exceptionhandling "Off"
	rtti "Off"
	flags { "FatalCompileWarnings" }
	
	linkhyperion()

	files {
		"%{prj.location}/**.hpp",
		"%{prj.location}/**.h",
		"%{prj.location}/**.c",
		"%{prj.location}/**.cpp"
	}
	excludes { "%{prj.location}/resource.rc" }
	includedirs { "%{prj.location}/include" }
		
    filter "system:windows"
		files { "%{prj.location}/resource.rc" }
		postbuildcommands {
		    "{COPY} %{cfg.targetdir}/%{prj.name}.exe ../../run_tree/hyperion.exe*"
	    }
