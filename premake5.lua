output_directory_format = "%{cfg.system}-%{cfg.architecture}"
audio_backend_option = "audio"
physics_backend_option = "physics"
with_mono_option = "with-mono"

include "packages.lua"

workspace "hyperion"
	startproject "hyperion-editor"

	targetdir ("build/%{cfg.buildcfg}/bin/" .. output_directory_format)
	objdir ("build/%{cfg.buildcfg}/obj/" .. output_directory_format)
	debugdir "run_tree/"

	configurations { "debug", "profile", "release" }

	newoption {
		trigger = audio_backend_option,
		value = "API",
		description = "Specifies the audio backend",
		allowed = {
			{ "none", "None" },
			{ "fmod", "FMOD" }
		}
	}
	if not _OPTIONS[audio_backend_option] then
		_OPTIONS[audio_backend_option] = "fmod"
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

	newoption {
		trigger = with_mono_option,
		description = "Specifies that the Mono scripting driver should be included",
	}

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
	filter "options:audio=fmod"
		defines { "HYP_AUDIO_FMOD" }

	filter "options:physics=none"
		defines { "HYP_PHYSICS_NONE" }
	filter "options:physics=bullet"
		defines { "HYP_PHYSICS_BULLET" }

	filter "options:with-mono"
		defines { "HYP_SCRIPTING_MONO" }

project "hyperion"
	location "projects/hyperion"
	dependson { "Hyperion.Core" }
	dependson { "Hyperion.Editor" }
	
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
		"%{prj.location}/include/hyperion/modules/fmod/**",
		"%{prj.location}/source/modules/fmod/**",
		"%{prj.location}/include/hyperion/modules/mono/**",
		"%{prj.location}/source/modules/mono/**",

        "%{prj.location}/vendor/glad/source/glad_wgl.c"
	}

	includedirs {
		"%{prj.location}/include",
		
		"%{prj.location}/vendor/glad/include"
	}
	includedirs { package_assimp_includedirs }
	includedirs { package_fmt_includedirs }
	includedirs { package_freetype_includedirs }
	includedirs { package_rapidjson_includedirs }
	includedirs { package_rttr_includedirs }
	includedirs { package_stb_includedirs }
			
	filter "files:projects/hyperion/vendor/**"
		flags { "NoPCH" }

	filter "options:audio=fmod"
		files { 
			"%{prj.location}/include/hyperion/modules/fmod/**",
			"%{prj.location}/source/modules/fmod/**"
		}
		includedirs { "%{prj.location}/vendor/fmod/include" }
	filter "options:physics=bullet"
		files { 
			"%{prj.location}/include/hyperion/modules/bullet/**",
			"%{prj.location}/source/modules/bullet/**"
		}
		includedirs { package_bullet_includedirs }
	filter "options:with-mono"
		files {
			"%{prj.location}/include/hyperion/modules/mono/**",
			"%{prj.location}/source/modules/mono/**",
		}
		includedirs { "%{prj.location}/vendor/mono/include" }

	filter "configurations:profile"
		files { "%{prj.location}/vendor/optick/source/**" }
		includedirs { "%{prj.location}/vendor/optick/include" }

	filter "system:windows"
		files {
			"%{prj.location}/include/hyperion/platform/windows/**.hpp",
			"%{prj.location}/source/platform/windows/**.cpp",

            "%{prj.location}/vendor/glad/source/glad_wgl.c"
		}
		
	    postbuildcommands {
		    "{COPY} vendor/fmod/lib/windows/fmod.dll %{cfg.targetdir}/fmod.dll*",
			"{COPY} vendor/mono/lib/windows/mono.dll %{cfg.targetdir}/mono.dll*"
	    }

function linkhyperion()
	filter { }

	includedirs { "projects/hyperion/include" }
	includedirs { package_rttr_includedirs }
	includedirs { package_fmt_includedirs }

	links { "hyperion" }
	
	filter "kind:ConsoleApp"
		defines { "HYP_CONSOLE" }

	filter "system:windows"
		links { "opengl32", "PowrProf" }
	
	filter { "system:windows", "configurations:debug" }
		libdirs { package_assimp_debug_libdirs }
		libdirs { package_fmt_debug_libdirs }
		libdirs { package_freetype_debug_libdirs }
		libdirs { package_rttr_debug_libdirs }
		links { package_assimp_debug_links }
		links { package_fmt_debug_links }
		links { package_freetype_debug_links }
		links { package_rttr_debug_links }
	filter { "system:windows", "configurations:profile or release" }
		libdirs { package_assimp_release_libdirs }
		libdirs { package_fmt_release_libdirs }
		libdirs { package_freetype_release_libdirs }
		libdirs { package_rttr_release_libdirs }
		links { package_assimp_release_links }
		links { package_fmt_release_links }
		links { package_freetype_release_links }
		links { package_rttr_release_links }

	filter { "system:windows", "options:audio=fmod" }
		links { "fmod_vc" }
		libdirs { "projects/hyperion/vendor/fmod/lib/windows" }
	filter { "system:windows", "configurations:debug", "options:physics=bullet" }
		libdirs { package_bullet_debug_libdirs }
		links { package_bullet_debug_links }
	filter { "system:windows", "configurations:profile or release", "options:physics=bullet" }
		libdirs { package_bullet_release_libdirs }
		links { package_bullet_release_links }
	filter { "system:windows", "options:with-mono" }
		links { "mono" }
		libdirs { "projects/hyperion/vendor/mono/lib/windows" }
		
	filter { }

end

project "hyperion-editor"
	location "projects/hyperion-editor"
	
	language "C++"
	cppdialect "C++17"
	architecture "x86_64"
	kind "ConsoleApp"
	
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
        files { "projects/hyperion-editor/resource.rc" }

		postbuildcommands {
		    "{COPY} %{cfg.targetdir}/%{prj.name}.exe ../../run_tree/hyperion.exe*",
		    
		    "{COPY} %{cfg.targetdir}/fmod.dll ../../run_tree/fmod.dll*",
			"{COPY} %{cfg.targetdir}/mono.dll ../../run_tree/mono.dll*"
	    }

group "managed"

project "Hyperion.Core"
	location "projects/managed/Hyperion.Core"
	
	language "C#"
	kind "SharedLib"
	architecture "x86_64"
	namespace ("Hyperion")
	
	targetdir ("build/%{cfg.buildcfg}/bin/managed/")
	objdir ("build/%{cfg.buildcfg}/obj/managed/")
	
	files { "%{prj.location}/**.cs" }
	
	postbuildcommands {
		"{COPY} $(TargetDir)$(TargetFileName) $(ProjectDir)../../../run_tree/data/managed/",
		"{COPY} $(TargetDir)$(TargetName).pdb $(ProjectDir)../../../run_tree/data/managed/"
	}
	
	filter "system:windows"
		postbuildcommands {
			"$(ProjectDir)../../../run_tree/data/tools/pdb2mdb.exe $(ProjectDir)../../../run_tree/data/managed/$(TargetFileName)"
		}

project "Hyperion.Editor"
	location "projects/managed/Hyperion.Editor"
	
	language "C#"
	kind "SharedLib"
	architecture "x86_64"
	namespace ("Hyperion.Editor")
	
	links ("Hyperion.Core")
	
	targetdir ("build/%{cfg.buildcfg}/bin/managed/")
	objdir ("build/%{cfg.buildcfg}/obj/managed/")
	
	files { "%{prj.location}/**.cs" }
	
	postbuildcommands {
		"{COPY} $(TargetDir)$(TargetFileName) $(ProjectDir)../../../run_tree/data/managed/",
		"{COPY} $(TargetDir)$(TargetName).pdb $(ProjectDir)../../../run_tree/data/managed/"
	}
	
	filter "system:windows"
		postbuildcommands {
			"$(ProjectDir)../../../run_tree/data/tools/pdb2mdb.exe $(ProjectDir)../../../run_tree/data/managed/$(TargetFileName)"
		}
