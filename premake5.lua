workspace "hyperion"
	language "C++"
	cppdialect "C++17"
	architecture "x64"
	staticruntime "On"
	startproject "sandbox"
	
	configurations
	{
		"Debug",
		"Release",
		"Distribution"
	}

	flags { "FatalCompileWarnings" }

	outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
	
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
		systemversion "latest"
		
		files
		{
			"%{prj.name}/src/platform/windows/**.cpp",
		}
		
		defines
		{
			"HYP_PLATFORM_WINDOWS"
		}
	
	filter "configurations:Debug"
		defines { "HYP_DEBUG" }
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines { "HYP_RELEASE" }
		runtime "Release"
		optimize "On"
		
	filter "configurations:Distribution"
		defines { "HYP_DISTRIBUTION" }
		runtime "Release"
		optimize "On"
		
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
		defines { "HYP_PLATFORM_WINDOWS" }
		systemversion "latest"
		
	filter "configurations:Debug"
		defines { "HYP_DEBUG" }
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines { "HYP_RELEASE" }
		runtime "Release"
		optimize "On"
		
	filter "configurations:Distribution"
		defines { "HYP_DISTRIBUTION" }
		runtime "Release"
		optimize "On"