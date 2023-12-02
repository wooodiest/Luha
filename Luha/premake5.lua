project "Luha"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "lhpch.h"
	pchsource "src/lhpch.cpp"

	files
    {
        "src/**.h",
		"src/**.cpp"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	includedirs
    {
		"src",
		"vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}"
	}

	links 
	{
		"GLFW",
		"Glad",
		"opengl32.lib"
	}

	filter "system:windows"
        systemversion "latest"
		defines
        { "GLFW_INCLUDE_NONE" }

		filter "configurations:Debug"
		defines "LH_DEBUG"
		runtime "Debug"
		symbols "On"

		filter "configurations:Release"
		defines "LH_RELEASE"
		runtime "Release"
		optimize "on"
		symbols "On"

		filter "configurations:Dist"
		defines "LH_DIST"
		runtime "Release"
		optimize "on"
		symbols "Off"