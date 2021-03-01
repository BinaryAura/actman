workspace "actman"
  configurations { "Debug" , "Release"}
  cppdialect "C++17"
  pic "On"

out_dir = "%{wks.location}/"
out_struct = "%{cfg.buildcfg}/"

include_dirs = {}

include "deps" -- include dependencies

project "actman"
  kind "ConsoleApp"
  language "C++"
  targetdir (out_dir .. "bin/" .. out_struct .. "%{prj.name}")
  objdir (out_dir .. "build/" .. out_struct .. "%{prj.name}")
  buildoptions {"-Wfatal-errors"}

  includedirs {
    "include",
    "%{include_dirs.hadron}",
    "deps/hadron-engine/%{include_dirs.spdlog}",
    "deps/hadron-engine/%{include_dirs.entt}",
    "deps/hadron-engine/%{include_dirs.glm}"
  }

  links {
    "ncurses",
    "hadron"
  }

  files {
    "src/actman.cpp",
    "src/boardconfig.cpp"
  }

  filter "configurations:Debug"
    defines { "DEBUG" }
    symbols "On"

  filter "configurations:Release"
    optimize "On"
