group "managed"

project "Hyperion"
  location "managed/Hyperion"

  language "C#"
  framework "net6.0"
  architecture "x86_64"
  csversion "9.0"
  kind "SharedLib"
  clr "Unsafe"

  dynamicloading(true)

  files { "%{prj.location}/**.cs" }

  excludes { "%{prj.location}/obj/**.cs" }

  postbuildcommands {
    "{COPY} Hyperion.dll ../../../../../run_tree/data/managed/Hyperion.dll*",
    "{COPY} Hyperion.pdb ../../../../../run_tree/data/managed/Hyperion.pdb*",
    "{COPY} Hyperion.deps.json ../../../../../run_tree/data/managed/Hyperion.deps.json*",
    "{COPY} Hyperion.runtimeconfig.json ../../../../../run_tree/data/managed/Hyperion.runtimeconfig.json*",
  }
