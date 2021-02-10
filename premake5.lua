workspace "actman"
  configurations { "Debug" }

project "actman"
  kind "ConsoleApp"
  language "C++"
  targetdir "bin/%{cfg}"

  files { "include/**.h", "src/**.cpp" }

  filter "configurations:Debug"
    defines { "DEBUG" }
    symbols "On"
