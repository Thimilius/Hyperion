workspace "hyperion"
	startproject "hyperion-editor"

	language "C++"
	cppdialect "C++17"
	architecture "x86_64"
	
	staticruntime "On"
	
	exceptionhandling ("Off")
	rtti ("Off")
	
	flags { "FatalCompileWarnings" }
	
	debugdir "run_tree/"
	
	outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
	targetdir ("build/bin/" .. outputdir)
	objdir ("build/obj/" .. outputdir)

	configurations { "debug", "release" }

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
	
project "hyperion"
	location "projects/hyperion"
	kind "StaticLib"
	
	pchheader "hyppch.hpp"
	pchsource "projects/hyperion/src/hyppch.cpp"

	files {
		"projects/hyperion/**.hpp",
		"projects/hyperion/**.h",
		"projects/hyperion/**.c",
		"projects/hyperion/**.cpp",
	}

	excludes {
		"projects/hyperion/src/platform/**",
		"projects/hyperion/include/hyperion/platform/**",

        "projects/hyperion/vendor/glad/src/glad_wgl.c"
	}

	includedirs {
		"projects/hyperion/include",
		
		"projects/hyperion/vendor/assimp/include",
		"projects/hyperion/vendor/bullet/include",
		"projects/hyperion/vendor/fmod/include",
		"projects/hyperion/vendor/fmt/include",
		"projects/hyperion/vendor/freetype/include",
		"projects/hyperion/vendor/glad/include",
		"projects/hyperion/vendor/nameof/include",
		"projects/hyperion/vendor/nlohmann/include",
		"projects/hyperion/vendor/mono/include",
		"projects/hyperion/vendor/rttr/include",
		"projects/hyperion/vendor/stb/include",
	}
	
	filter "files:projects/hyperion/vendor/**"
		flags { "NoPCH" }

	filter "system:windows"
		files {
			"projects/hyperion/include/hyperion/platform/windows/**.hpp",
			"projects/hyperion/src/platform/windows/**.cpp",

            "projects/hyperion/vendor/glad/src/glad_wgl.c"
		}
		
	    postbuildcommands {
		    "{COPY} vendor/fmod/lib/windows/fmod.dll %{cfg.targetdir}",
		    "{COPY} vendor/assimp/lib/windows/assimp.dll %{cfg.targetdir}",
			"{COPY} vendor/mono/lib/windows/mono.dll %{cfg.targetdir}"
	    }

group "tools"	

project "hyperion-editor"
	location "projects/tools/hyperion-editor"
	kind "ConsoleApp"
	
	links { "hyperion" }
	
	files {
		"projects/tools/hyperion-editor/**.hpp",
		"projects/tools/hyperion-editor/**.h",
		"projects/tools/hyperion-editor/**.c",
		"projects/tools/hyperion-editor/**.cpp",
	}
	
    excludes { "projects/tools/hyperion-editor/resource.rc" }

	includedirs {
		"projects/tools/hyperion-editor/include",
	
		"projects/hyperion/include",
		
		"projects/hyperion/vendor/fmt/include",
		"projects/hyperion/vendor/nameof/include",
		"projects/hyperion/vendor/rttr/include",
	}
		
	filter "kind:ConsoleApp"
		defines { "HYP_CONSOLE" }

    filter "system:windows"
        files { "projects/tools/hyperion-editor/resource.rc" }

		libdirs {
			"projects/hyperion/vendor/fmod/lib/windows",
			"projects/hyperion/vendor/assimp/lib/windows",
			"projects/hyperion/vendor/mono/lib/windows"
		}

		links {
			"opengl32",
			"PowrProf",
		
			"rttr",
			"freetype",
			"bullet",
		
			"fmod_vc",
			"assimp",
			"mono",	
		}

	    postbuildcommands {
		    "{COPY} %{cfg.targetdir}/%{prj.name}.exe ../../../run_tree/",
		    
		    "{COPY} %{cfg.targetdir}/fmod.dll ../../../run_tree/",
		    "{COPY} %{cfg.targetdir}/assimp.dll ../../../run_tree/",
			"{COPY} %{cfg.targetdir}/mono.dll ../../../run_tree/",
	    }

	filter { "system:windows", "configurations:debug" }
		libdirs { "projects/hyperion/vendor/bullet/lib/windows/debug" }
		libdirs { "projects/hyperion/vendor/freetype/lib/windows/debug" }
		libdirs { "projects/hyperion/vendor/rttr/lib/windows/debug" }
		
	filter { "system:windows", "configurations:release" }
		libdirs { "projects/hyperion/vendor/bullet/lib/windows/release" }
		libdirs { "projects/hyperion/vendor/freetype/lib/windows/release" }
		libdirs { "projects/hyperion/vendor/rttr/lib/windows/release" }

group "managed"

project "HyperionEngine"
	location "projects/managed/HyperionEngine"
	kind "SharedLib"
	
	language "C#"
	
	files { "projects/managed/HyperionEngine/**.cs" }
	
	postbuildcommands {
		"{COPY} $(TargetDir)$(TargetFileName) $(ProjectDir)../../../run_tree/data/managed/"
	}
