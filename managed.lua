group "managed"

project "Hyperion.Core"
  location "hyperion-managed/Hyperion.Core"

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
  namespace "Hyperion"

  files { "%{prj.location}/**.cs" }
  excludes { "%{prj.location}/obj/**.cs" }

  postbuildcommands {
    "{COPY} Hyperion.Core.dll ../../../../../run_tree/data/managed/",
    "{COPY} Hyperion.Core.pdb ../../../../../run_tree/data/managed/",
    "{COPY} Hyperion.Core.deps.json ../../../../../run_tree/data/managed/",
    "{COPY} Hyperion.Core.runtimeconfig.json ../../../../../run_tree/data/managed/",
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

  links { "Hyperion.Core" }

  files { "%{prj.location}/**.cs" }
  excludes { "%{prj.location}/obj/**.cs" }

  postbuildcommands {
    "{COPY} Hyperion.Sandbox.dll ../../../../../run_tree/data/managed/",
    "{COPY} Hyperion.Sandbox.pdb ../../../../../run_tree/data/managed/",
    "{COPY} Hyperion.Sandbox.deps.json ../../../../../run_tree/data/managed/",
  }
