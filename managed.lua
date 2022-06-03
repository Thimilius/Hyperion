group "managed"

project "Hyperion"
  location "hyperion-managed/Hyperion"

  configurations { }
  configurations { "Debug", "Release" }

  configmap {
    ["debug"] = "Debug",
    ["profile"] = "Release",
    ["release"] = "Release"
  }

  language "C#"
  framework "net6.0"
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

project "Hyperion.Sandbox"
  location "hyperion-managed/Hyperion.Sandbox"

  configurations { }
  configurations { "Debug", "Release" }

  configmap {
    ["debug"] = "Debug",
    ["profile"] = "Release",
    ["release"] = "Release"
  }

  language "C#"
  framework "net6.0"
  csversion "9.0"
  kind "SharedLib"

  dynamicloading(true)

  files { "%{prj.location}/**.cs" }

  excludes { "%{prj.location}/obj/**.cs" }

  postbuildcommands {
    "{COPY} Hyperion.Sandbox.dll ../../../../../run_tree/data/managed/Hyperion.Sandbox.dll*",
    "{COPY} Hyperion.Sandbox.pdb ../../../../../run_tree/data/managed/Hyperion.Sandbox.pdb*",
    "{COPY} Hyperion.Sandbox.deps.json ../../../../../run_tree/data/managed/Hyperion.Sandbox.deps.json*",
  }
