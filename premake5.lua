workspace "Neurex"
	architecture "x64"

	configurations {"Debug", "Release", "Dist"}

startproject "NXSandbox"
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

require "cmake"
require "clion"

include_dirs = {}
include_dirs["glad"] = "Neurex/vendor/glad/include"
include_dirs["spdlog"] = "Neurex/vendor/spdlog/include"
include_dirs["ImGui"] = "Neurex/vendor/imgui"
include_dirs["glm"] = "Neurex/vendor/glm"
include_dirs["stb_image"] = "Neurex/vendor/stb_image"
include_dirs["entt"] = "Neurex/vendor/entt/include"

group "Dependencies"
	include "Neurex/vendor/glad"
	include "Neurex/vendor/imgui"
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
		"%{prj.name}/vendor/glm/glm/**.inl",
		"%{prj.name}/vendor/stb_image/stb_image.h",
		"%{prj.name}/vendor/stb_image/stb_image.cpp",
	}

	includedirs
	{
		"%{prj.name}/src",
		"/usr/local/include",
		"%{include_dirs.spdlog}",
		"%{include_dirs.glad}",
		"%{include_dirs.ImGui}",
		"%{include_dirs.glm}",
		"%{include_dirs.stb_image}",
		"%{include_dirs.entt}"
	}


	filter "system:windows"
		systemversion "latest"
		links {
			"GLFW",
			"glad",
			"ImGui",
			"opengl32",
			"yaml-cpp"
		}

		defines {
			"NX_PT_WIN",
			"GLFW_INCLUDE_NONE",
			"_CRT_SECURE_NO_WARNINGS"
		}

	filter "system:macosx"
		links {
			"glfw3",
			"glad",
			"ImGui",
			"yaml-cpp",
			"Cocoa.framework",
			"CoreVideo.framework",
			"OpenGL.framework",
			"IOKit.framework",
		}

		defines {
			"NX_PT_OSX",
			"GLFW_INCLUDE_NONE",
			"_CRT_SECURE_NO_WARNINGS"
		}

	filter "configurations:Debug"
		defines {
			"NX_DEBUG",
			"NX_PROFILE"
		}
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

	includedirs {
		"%{include_dirs.spdlog}",
		"Neurex/src",
		"Neurex/vendor",
		"%{include_dirs.glm}",
		"%{include_dirs.entt}"
	}

	links {
		"Neurex"
	}

	filter "system:windows"
		systemversion "latest"

		defines { "NX_PT_WIN" }

	filter "system:macosx"
		links {
			"glfw3",
			"glad",
			"ImGui",
			"yaml-cpp",
			"Cocoa.framework",
			"CoreVideo.framework",
			"OpenGL.framework",
			"IOKit.framework",
		}

		defines {
			"NX_PT_OSX",
			"GLFW_INCLUDE_NONE",
			"_CRT_SECURE_NO_WARNINGS"	
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
		
project "Neutralise"
	location "Neutralise"
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

	includedirs {
		"%{include_dirs.spdlog}",
		"Neurex/src",
		"Neurex/vendor",
		"%{include_dirs.glm}",
		"%{include_dirs.entt}"
	}

	links {
		"Neurex"
	}

	filter "system:windows"
		systemversion "latest"

		defines { "NX_PT_WIN" }

	filter "system:macosx"
		links {
			"glfw3",
			"glad",
			"ImGui",
			"yaml-cpp",
			"Cocoa.framework",
			"CoreVideo.framework",
			"OpenGL.framework",
			"IOKit.framework",
		}

		defines {
			"NX_PT_OSX",
			"GLFW_INCLUDE_NONE",
			"_CRT_SECURE_NO_WARNINGS"	
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
