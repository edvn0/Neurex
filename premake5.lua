workspace "Neurex"
	architecture "x64"

	configurations {"Debug", "Release", "Dist"}

startproject "NXSandbox"
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

includeDir = {}
includeDir["GLFW"] = "Neurex/vendor/GLFW/include"
includeDir["glad"] = "Neurex/vendor/glad/include"
includeDir["spdlog"] = "Neurex/vendor/spdlog/include"
includeDir["ImGui"] = "Neurex/vendor/ImGui"
includeDir["glm"] = "Neurex/vendor/glm"

group "Dependencies"
	include "Neurex/vendor/GLFW"
	include "Neurex/vendor/glad"
	include "Neurex/vendor/ImGui"
group ""


project "Neurex"
	location "Neurex"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
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
		"%{includeDir.spdlog}",
		"%{includeDir.GLFW}",
		"%{includeDir.glad}",
		"%{includeDir.ImGui}",
		"%{includeDir.glm}"
	}


	filter "system:windows"
		systemversion "latest"

		links
		{
			"GLFW",
			"glad",
			"ImGui",
			"opengl32.lib",
		}

		defines 
		{
			"NX_PT_WIN",
			"GLFW_INCLUDE_NONE",
			"_CRT_SECURE_NO_WARNINGS"
		}

	filter "system:macosx"
		links 
		{
			"GLFW",
			"glad",
			"ImGui",
			"Cocoa.framework",
			"CoreVideo.framework",
			"OpenGL.framework",
			"IOKit.framework"
		}

		defines {
			"NX_PT_OSX",
			"GLFW_INCLUDE_NONE",
			"_CRT_SECURE_NO_WARNINGS"	
		}

	filter "configurations:Debug"
		defines "NX_DEBUG"
		symbols "on"
		runtime "Debug"

	filter "configurations:Release"
		defines "NX_RELEASE"
		optimize "on"
		runtime "Release"

	filter "configurations:Dist"
		defines "NX_DIST"
		optimize "on"
		runtime "Release"



project "NXSandbox"
	location "NXSandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs	{
		"Neurex/vendor/spdlog/include",
		"Neurex/src",
		"Neurex/vendor",
		"%{includeDir.glm}",
	}

	links {
		"Neurex"
	}

	filter "system:windows"
		systemversion "latest"

		defines { "NX_PT_WIN" }

	filter "system:macosx"
		defines {
			"NX_PT_OSX"
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