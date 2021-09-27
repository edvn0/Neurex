workspace "Neurex"
	architecture "x64"

	configurations {"Debug", "Release", "Dist"}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

includeDir = {}
includeDir["GLFW"] = "Neurex/vendor/GLFW/include"
includeDir["glad"] = "Neurex/vendor/glad/include"

include "Neurex/vendor/GLFW"
include "Neurex/vendor/glad"

project "Neurex"
	location "Neurex"
	kind "SharedLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "nxpch.h"
	pchsource "Neurex/src/nxpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{includeDir.GLFW}",
		"%{includeDir.glad}"
	}

	links
	{
		"GLFW",
		"glad",
		"opengl32.lib",
		"glu32.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines 
		{
			"NX_PT_WIN",
			"NX_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/NXSandbox")
		}

	filter "configurations:Debug"
		defines "NX_DEBUG"
		symbols "On"
		buildoptions "/MDd"

		defines 
		{
			"NX_ALLOW_ASSERTS"
		}

	filter "configurations:Release"
		defines "NX_RELEASE"
		optimize "On"
		buildoptions "/MD"

		defines 
		{
			"NX_ALLOW_ASSERTS"
		}

	filter "configurations:Dist"
		defines "NX_DIST"
		optimize "On"
		buildoptions "/MD"



project "NXSandbox"
	location "NXSandbox"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"Neurex/vendor/spdlog/include",
		"Neurex/src"
	}

	links 
	{
		"Neurex"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines { "NX_PT_WIN" }

	filter "configurations:Debug"
		defines "NX_DEBUG"
		symbols "On"
		buildoptions "/MDd"

	filter "configurations:Release"
		defines "NX_RELEASE"
		optimize "On"
		buildoptions "/MD"

	filter "configurations:Dist"
		defines "NX_DIST"
		optimize "On"
		buildoptions "/MD"