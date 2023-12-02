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
		"src"
	}

	filter "system:windows"
        systemversion "latest"

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