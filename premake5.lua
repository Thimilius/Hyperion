workspace "hyperion"
	language "C++"
	cppdialect "C++17"
	architecture "x64"
	staticruntime "On"
	startproject "sandbox"
	
	configurations
	{
		"debug",
		"release",
		"distribution"
	}

	flags { "FatalCompileWarnings" }

	outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

	filter "system:windows"
		defines { "HYP_PLATFORM_WINDOWS" }
		systemversion "latest"

	filter "configurations:debug"
		defines { "HYP_DEBUG", "HYP_BREAK_ON_ASSERT" }
		runtime "Debug"
		symbols "On"

	filter "configurations:release"
		defines { "HYP_RELEASE" }
		runtime "Release"
		optimize "On"
		
	filter "configurations:distribution"
		defines { "HYP_DISTRIBUTION" }
		runtime "Release"
		optimize "On"
	
project "hyperion"
	location "hyperion"
	kind "StaticLib"
	
	targetdir ("bin/" .. outputdir)
	objdir ("bin_int/" .. outputdir)

	pchheader "hyppch.hpp"
	pchsource "%{prj.name}/src/hyppch.cpp"

	debugdir "run_tree/"

	files
	{ 
		"%{prj.name}/include/**.hpp",
		
		"%{prj.name}/src/**.hpp",
		"%{prj.name}/src/**.cpp",
		
		"%{prj.name}/vendor/**.c",
		"%{prj.name}/vendor/**.h",
	}

	excludes
	{
		"%{prj.name}/src/%{prj.name}/platform/windows/**.hpp",
		"%{prj.name}/src/%{prj.name}/platform/windows/**.cpp",
	}

	includedirs
	{
		"%{prj.name}/include",
		"%{prj.name}/src",
		"%{prj.name}/vendor/glad/include",
		"%{prj.name}/vendor/stb/include",
		"%{prj.name}/vendor/freetype/include",
	}
	
	links
	{
		"opengl32",
		"freetype"
	}
	
	libdirs
	{
		"%{prj.name}/vendor/freetype"
	}
	
	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}
	
	filter "files:hyperion/vendor/**.c"
		flags { "NoPCH" }

	filter "system:windows"
		files
		{
			"%{prj.name}/src/%{prj.name}/platform/windows/**.hpp",
			"%{prj.name}/src/%{prj.name}/platform/windows/**.cpp",
		}
		
project "sandbox"
	location "sandbox"
	kind "ConsoleApp"
	
	links { "hyperion" }
	
	targetdir ("bin/" .. outputdir)
	objdir ("bin_int/" .. outputdir)
	
	debugdir "run_tree/"

	postbuildcommands
	{
		"{COPY} %{cfg.targetdir}/%{prj.name}.exe ../run_tree/%{prj.name}.exe*"
	}
	
	files
	{ 
		"%{prj.name}/src/**.hpp",
		"%{prj.name}/src/**.cpp",
	}	
	
	includedirs
	{
		"hyperion/include"
	}
	
	filter "system:windows"
		links
		{
			"PowrProf"
		}
		
	filter "kind:ConsoleApp"
		defines { "HYP_CONSOLE" }