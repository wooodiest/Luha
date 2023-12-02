workspace "LuhaApp"
    architecture "x64"
    configurations { "Debug", "Release", "Dist" }
    startproject "LuhaApp"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "Luha"
include "LuhaApp"