project "hyperion-editor"
	location "hyperion-editor"
	
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
			"{COPY} %{cfg.targetdir}/%{prj.name}.exe ../run_tree/hyperion.exe*"
		}