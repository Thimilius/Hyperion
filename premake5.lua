output_directory_format = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
audio_backend_option = "audio"
physics_backend_option = "physics"

workspace "hyperion"
	startproject "hyperion-editor"

	language "C++"
	cppdialect "C++17"
	architecture "x86_64"
	
	staticruntime "On"
	exceptionhandling "Off"
	rtti "Off"
	flags { "FatalCompileWarnings" }
	
	targetdir ("build/bin/" .. output_directory_format)
	objdir ("build/obj/" .. output_directory_format)
	debugdir "run_tree/"

	configurations { "debug", "release" }

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

	filter "configurations:debug"
		defines { "HYP_DEBUG", "HYP_ENABLE_ASSERTS", "HYP_BREAK_ON_ASSERT" }
		runtime "Debug"
		symbols "On"
	filter "configurations:release"
		defines { "HYP_RELEASE" }
		runtime "Release"
		optimize "On"
	
	filter "system:windows"
		defines { "HYP_PLATFORM_WINDOWS", "_CRT_SECURE_NO_WARNINGS", "_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS" }
		systemversion "latest"

	filter "options:audio=none"
		defines { "HYP_AUDIO_NONE" }
	filter "options:audio=fmod"
		defines { "HYP_AUDIO_FMOD" }

	filter "options:physics=none"
		defines { "HYP_PHYSICS_NONE" }
	filter "options:physics=bullet"
		defines { "HYP_PHYSICS_BULLET" }

project "hyperion"
	location "projects/hyperion"
	kind "StaticLib"
	
	pchheader "hyppch.hpp"
	pchsource "%{prj.location}/src/hyppch.cpp"

	files {
		"%{prj.location}/**.hpp",
		"%{prj.location}/**.h",
		"%{prj.location}/**.c",
		"%{prj.location}/**.cpp"
	}
	excludes {
		"%{prj.location}/include/hyperion/platform/**",
		"%{prj.location}/src/platform/**",

		"%{prj.location}/include/hyperion/modules/bullet/**",
		"%{prj.location}/src/modules/bullet/**",
		"%{prj.location}/include/hyperion/modules/fmod/**",
		"%{prj.location}/src/modules/fmod/**",

        "%{prj.location}/vendor/glad/src/glad_wgl.c"
	}

	includedirs {
		"%{prj.location}/include",
		
		"%{prj.location}/vendor/assimp/include",
		"%{prj.location}/vendor/fmt/include",
		"%{prj.location}/vendor/freetype/include",
		"%{prj.location}/vendor/glad/include",
		"%{prj.location}/vendor/nameof/include",
		"%{prj.location}/vendor/nlohmann/include",
		"%{prj.location}/vendor/mono/include",
		"%{prj.location}/vendor/rttr/include",
		"%{prj.location}/vendor/stb/include"
	}
	
	filter "files:projects/hyperion/vendor/**"
		flags { "NoPCH" }

	filter "options:audio=fmod"
		files { 
			"%{prj.location}/include/hyperion/modules/fmod/**",
			"%{prj.location}/src/modules/fmod/**"
		}
		includedirs { "%{prj.location}/vendor/fmod/include" }
	filter "options:physics=bullet"
		files { 
			"%{prj.location}/include/hyperion/modules/bullet/**",
			"%{prj.location}/src/modules/bullet/**"
		}
		includedirs { "%{prj.location}/vendor/bullet/include" }

	filter "system:windows"
		files {
			"%{prj.location}/include/hyperion/platform/windows/**.hpp",
			"%{prj.location}/src/platform/windows/**.cpp",

            "%{prj.location}/vendor/glad/src/glad_wgl.c"
		}
		
	    postbuildcommands {
		    "{COPY} vendor/fmod/lib/windows/fmod.dll %{cfg.targetdir}",
		    "{COPY} vendor/assimp/lib/windows/assimp.dll %{cfg.targetdir}",
			"{COPY} vendor/mono/lib/windows/mono.dll %{cfg.targetdir}"
	    }

function linkhyperion()
	filter { }

	links { "hyperion" }
	
	includedirs {
		"projects/hyperion/include",
		
		"projects/hyperion/vendor/fmt/include",
		"projects/hyperion/vendor/nameof/include",
		"projects/hyperion/vendor/rttr/include"
	}

	filter "kind:ConsoleApp"
		defines { "HYP_CONSOLE" }

	filter "system:windows"
		libdirs {
			"projects/hyperion/vendor/assimp/lib/windows",
			"projects/hyperion/vendor/mono/lib/windows"
		}

		links {
			"opengl32",
			"PowrProf",
		
			"rttr",
			"freetype",
		
			"assimp",
			"mono",	
		}
	
	filter { "system:windows", "configurations:debug" }
		libdirs {
			"projects/hyperion/vendor/freetype/lib/windows/debug",
			"projects/hyperion/vendor/rttr/lib/windows/debug"
		}
	filter { "system:windows", "configurations:release" }
		libdirs {
			"projects/hyperion/vendor/freetype/lib/windows/release",
			"projects/hyperion/vendor/rttr/lib/windows/release"
		}

	filter { "system:windows", "options:audio=fmod" }
		links { "fmod_vc" }
		libdirs { "projects/hyperion/vendor/fmod/lib/windows" }

	filter { "system:windows", "options:physics=bullet" }
		links { "bullet" }
	filter { "system:windows", "configurations:debug", "options:physics=bullet" }
		libdirs { "projects/hyperion/vendor/bullet/lib/windows/debug" }
	filter { "system:windows", "configurations:release", "options:physics=bullet" }
		libdirs { "projects/hyperion/vendor/bullet/lib/windows/release" }

	filter { }

end

project "hyperion-editor"
	location "projects/tools/hyperion-editor"
	kind "ConsoleApp"
	
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
        files { "projects/tools/hyperion-editor/resource.rc" }

		postbuildcommands {
		    "{COPY} %{cfg.targetdir}/%{prj.name}.exe ../../../run_tree/",
		    
		    "{COPY} %{cfg.targetdir}/fmod.dll ../../../run_tree/",
		    "{COPY} %{cfg.targetdir}/assimp.dll ../../../run_tree/",
			"{COPY} %{cfg.targetdir}/mono.dll ../../../run_tree/"
	    }

group "managed"

project "HyperionEngine"
	location "projects/managed/HyperionEngine"
	kind "SharedLib"
	
	language "C#"
	
	files { "%{prj.location}/**.cs" }
	
	postbuildcommands {
		"{COPY} $(TargetDir)$(TargetFileName) $(ProjectDir)../../../run_tree/data/managed/"
	}
