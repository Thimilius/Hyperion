output_directory_format = "%{cfg.system}-%{cfg.architecture}"
audio_backend_option = "audio"
physics_backend_option = "physics"

include "packages.lua"

workspace "hyperion"
	startproject "hyperion-editor"

	targetdir ("build/%{cfg.buildcfg}/bin/" .. output_directory_format)
	objdir ("build/%{cfg.buildcfg}/obj/" .. output_directory_format)
	debugdir "run_tree/"

	configurations { "debug", "profile", "release" }

	newoption {   
		trigger = "editor",   
		description = "Compiles the engine with editor support"
	}

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
		buildoptions { "/MP /utf-8" }

	filter "options:editor"
		defines { "HYP_EDITOR" }

	filter "options:audio=none"
		defines { "HYP_AUDIO_NONE" }

	filter "options:physics=none"
		defines { "HYP_PHYSICS_NONE" }
	filter "options:physics=bullet"
		defines { "HYP_PHYSICS_BULLET" }

include "project.lua"

project "hyperion-sandbox"
	location "projects/hyperion-sandbox"
	
	language "C++"
	cppdialect "C++20"
	architecture "x86_64"
	kind "WindowedApp"
	
	staticruntime "On"
	exceptionhandling "Off"
	rtti "Off"
	flags { "FatalCompileWarnings" }
	
	linkhyperion ""

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
		
project "hyperion-editor"
	location "projects/hyperion-editor"
	
	language "C++"
	cppdialect "C++20"
	architecture "x86_64"
	kind "WindowedApp"
	
	staticruntime "On"
	exceptionhandling "Off"
	rtti "Off"
	flags { "FatalCompileWarnings" }

	linkhyperion ""

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
