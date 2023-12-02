include "Dependencies.lua"

workspace "LuhaApp"
    architecture "x64"
    configurations { "Debug", "Release", "Dist" }
    flags { "MultiProcessorCompile" }
    startproject "LuhaApp"

    filter "system:windows"
    buildoptions { "/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus" }
    
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
	include "Luha/vendor/GLFW"
	include "Luha/vendor/Glad"
group ""

include "Luha"
include "LuhaApp"