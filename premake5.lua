workspace "hyperion"
	startproject "sandbox"

	language "C++"
	cppdialect "C++17"
	architecture "x86_64"
	
	staticruntime "On"
	
	exceptionhandling ("Off")
	rtti ("Off")
	
	flags { "FatalCompileWarnings" }
	
	configurations { "debug", "release" }

	outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
	targetdir ("build/bin/" .. outputdir)
	objdir ("build/obj/" .. outputdir)

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

	debugdir "run_tree/"

	postbuildcommands {
		"{COPY} vendor/fmod/lib/x64/fmod.dll %{cfg.targetdir}",
		"{COPY} vendor/assimp/lib/x64/assimp-vc140-mt.dll %{cfg.targetdir}"
	}

	files { "projects/hyperion/**" }

	excludes {
		"projects/hyperion/src/platform/**",
		"projects/hyperion/include/hyperion/platform/**",
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
	
	links {	"freetype", "fmod_vc", "assimp-vc140-mt" }
	
	libdirs {
		"projects/hyperion/vendor/freetype/lib/x64",
		"projects/hyperion/vendor/fmod/lib/x64",
		"projects/hyperion/vendor/assimp/lib/x64",
	}
	
	filter "files:projects/hyperion/vendor/**"
		flags { "NoPCH" }

	filter "system:windows"
		files {
			"projects/hyperion/include/hyperion/platform/windows/**",
			"projects/hyperion/src/platform/windows/**"
		}
		
		links { "opengl32", "PowrProf" }
		linkoptions { "-IGNORE:4006" }



project "sandbox"
	location "projects/sandbox"
	kind "ConsoleApp"
	
	links { "hyperion" }
	
	debugdir "run_tree/"

	postbuildcommands {
		"{COPY} %{cfg.targetdir}/%{prj.name}.exe ../../run_tree/",
		
		"{COPY} %{cfg.targetdir}/fmod.dll ../../run_tree/",
		"{COPY} %{cfg.targetdir}/assimp-vc140-mt.dll ../../run_tree/"
	}
	
	files { "projects/sandbox/**" }	
	
	includedirs {
		"projects/hyperion/include",
		
		"projects/hyperion/vendor/fmt/include",
	}
		
	filter "kind:ConsoleApp"
		defines { "HYP_CONSOLE" }