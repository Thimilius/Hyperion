workspace "hyperion"
	startproject "sandbox"

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
		defines { "HYP_PLATFORM_WINDOWS", "_CRT_SECURE_NO_WARNINGS" }
		systemversion "latest"
	
project "hyperion"
	location "projects/hyperion"
	kind "StaticLib"
	
	pchheader "hyppch.hpp"
	pchsource "projects/hyperion/src/hyppch.cpp"

	files { "projects/hyperion/**" }

	excludes {
		"projects/hyperion/src/platform/**",
		"projects/hyperion/include/hyperion/platform/**",

        "projects/hyperion/vendor/glad/src/glad_wgl.c"
	}

	includedirs {
		"projects/hyperion/include",
		
		"projects/hyperion/vendor/glad/include",
		"projects/hyperion/vendor/stb/include",
		"projects/hyperion/vendor/fmt/include",
		"projects/hyperion/vendor/freetype/include",
		"projects/hyperion/vendor/fmod/include",
		"projects/hyperion/vendor/assimp/include",
	}
	
	filter "files:projects/hyperion/vendor/**"
		flags { "NoPCH" }

	filter "system:windows"
		files {
			"projects/hyperion/include/hyperion/platform/windows/**",
			"projects/hyperion/src/platform/windows/**",

            "projects/hyperion/vendor/glad/src/glad_wgl.c"
		}
		
		libdirs {
			"projects/hyperion/vendor/freetype/lib/windows",
			"projects/hyperion/vendor/fmod/lib/windows",
			"projects/hyperion/vendor/assimp/lib/windows",
		}
		
		links {
			"opengl32",
			"PowrProf",
		
			"freetype",
			"fmod_vc",
			"assimp"
		}
		
		linkoptions { "-IGNORE:4006" }

	    postbuildcommands {
		    "{COPY} vendor/fmod/lib/x64/fmod.dll %{cfg.targetdir}",
		    "{COPY} vendor/assimp/lib/windows/assimp.dll %{cfg.targetdir}"
	    }

project "sandbox"
	location "projects/sandbox"
	kind "ConsoleApp"
	
	links { "hyperion" }
	
	files { "projects/sandbox/**" }	
	
    excludes { "projects/sandbox/resource.rc" }

	includedirs {
		"projects/hyperion/include",
		
		"projects/hyperion/vendor/fmt/include",
	}
		
	filter "kind:ConsoleApp"
		defines { "HYP_CONSOLE" }

    filter "system:Windows"
        files { "projects/sandbox/resource.rc" }

	    postbuildcommands {
		    "{COPY} %{cfg.targetdir}/%{prj.name}.exe ../../run_tree/",
		    
		    "{COPY} %{cfg.targetdir}/fmod.dll ../../run_tree/",
		    "{COPY} %{cfg.targetdir}/assimp.dll ../../run_tree/"
	    }
	
