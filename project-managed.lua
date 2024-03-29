group "managed"

project "Hyperion.Runtime"
  location "hyperion-managed/Hyperion.Runtime"

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
    "{MKDIR} ../../../../../run_tree/managed/",
    "{COPY} Hyperion.Runtime.dll ../../../../../run_tree/managed/",
    "{COPY} Hyperion.Runtime.pdb ../../../../../run_tree/managed/",
    "{COPY} Hyperion.Runtime.deps.json ../../../../../run_tree/managed/",
    "{COPY} Hyperion.Runtime.runtimeconfig.json ../../../../../run_tree/managed/",
  }

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
  namespace "Hyperion"

  files { "%{prj.location}/**.cs" }
  excludes { "%{prj.location}/obj/**.cs" }

  postbuildcommands {
    "{MKDIR} ../../../../../run_tree/managed/",
    "{COPY} Hyperion.Core.dll ../../../../../run_tree/managed/",
    "{COPY} Hyperion.Core.pdb ../../../../../run_tree/managed/",
    "{COPY} Hyperion.Core.deps.json ../../../../../run_tree/managed/",
  }

  project "Hyperion.Editor"
  location "hyperion-managed/Hyperion.Editor"

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

  links { "Hyperion.Core" }

  files { "%{prj.location}/**.cs" }
  excludes { "%{prj.location}/obj/**.cs" }

  postbuildcommands {
    "{MKDIR} ../../../../../run_tree/managed/",
    "{COPY} Hyperion.Editor.dll ../../../../../run_tree/managed/",
    "{COPY} Hyperion.Editor.pdb ../../../../../run_tree/managed/",
    "{COPY} Hyperion.Editor.deps.json ../../../../../run_tree/managed/",
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

  links { "Hyperion.Core" }

  files { "%{prj.location}/**.cs" }
  excludes { "%{prj.location}/obj/**.cs" }

  postbuildcommands {
    "{MKDIR} ../../../../../run_tree/managed/",
    "{COPY} Hyperion.Sandbox.dll ../../../../../run_tree/managed/",
    "{COPY} Hyperion.Sandbox.pdb ../../../../../run_tree/managed/",
    "{COPY} Hyperion.Sandbox.deps.json ../../../../../run_tree/managed/",
  }
