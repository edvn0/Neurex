workspace "Neurex"
	architecture "x64"

	configurations {"Debug", "Release", "Dist"}

startproject "NXSandbox"
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

includeDir = {}
includeDir["GLFW"] = "Neurex/vendor/GLFW/include"
includeDir["glad"] = "Neurex/vendor/glad/include"
includeDir["imgui"] = "Neurex/vendor/imgui"
includeDir["glm"] = "Neurex/vendor/glm"

group "Dependencies"
	include "Neurex/vendor/GLFW"
	include "Neurex/vendor/glad"
	include "Neurex/vendor/imgui"
group ""


project "Neurex"
	location "Neurex"
	kind "SharedLib"
	language "C++"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "nxpch.h"
	pchsource "Neurex/src/nxpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{includeDir.GLFW}",
		"%{includeDir.glad}",
		"%{includeDir.imgui}",
		"%{includeDir.glm}"
	}

	links
	{
		"GLFW",
		"glad",
		"imgui",
		"opengl32.lib",
		"glu32.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines 
		{
			"NX_PT_WIN",
			"NX_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/NXSandbox/\"")
		}

	filter "configurations:Debug"
		defines "NX_DEBUG"
		symbols "On"
		runtime "Debug"

	filter "configurations:Release"
		defines "NX_RELEASE"
		optimize "On"
		runtime "Release"

	filter "configurations:Dist"
		defines "NX_DIST"
		optimize "On"
		runtime "Release"



project "NXSandbox"
	location "NXSandbox"
	kind "ConsoleApp"
	language "C++"
	staticruntime "off"

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
		"Neurex/src",
		"%{includeDir.glm}"
	}

	links 
	{
		"Neurex"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines { "NX_PT_WIN" }

	filter "configurations:Debug"
		defines "NX_DEBUG"
		symbols "On"
		runtime "Debug"

	filter "configurations:Release"
		defines "NX_RELEASE"
		optimize "On"
		runtime "Release"

	filter "configurations:Dist"
		defines "NX_DIST"
		optimize "On"
		runtime "Release"
