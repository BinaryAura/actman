workspace "actman"
  configurations { "Debug" , "Release"}
  cppdialect "C++17"
  pic "On"

out_dir = "%{wks.location}/"
out_struct = "%{cfg.buildcfg}/"

include_dirs = {}
include_dirs["spdlog"] = "deps/spdlog/include"


include "deps/spdlog"


project "actman"
  kind "ConsoleApp"
  language "C++"
  targetdir (out_dir .. "bin/" .. out_struct .. "%{prj.name}")
  objdir (out_dir .. "build/" .. out_struct .. "%{prj.name}")
  buildoptions {"-Wfatal-errors"}

  includedirs {
    "include",
    "%{include_dirs.spdlog}"
  }

  links {
    "spdlog"
  }

  files {
    "src/main.cpp",
    "src/log.cpp",
    "src/actman.cpp",
    "src/boardconfig.cpp"
  }

  filter "configurations:Debug"
    defines { "DEBUG" }
    symbols "On"

  filter "configurations:Release"
    optimize "On"
