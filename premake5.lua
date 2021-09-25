workspace "Neurex"
	architecture "x64"

	configurations {"Debug", "Release", "Dist"}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Neurex"
	location "Neurex"
	kind "SharedLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.hpp",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/vendor/spdlog/include"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines 
		{
			"NX_PT_WIN",
			"NX_BUILD_DLL"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/NXSandbox")
		}

	filter "configurations:Debug"
		defines "NX_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "NX_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "NX_DIST"
		optimize "On"



project "NXSandbox"
	location "NXSandbox"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.hpp",
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

		defines 
		{
			"NX_PT_WIN"
		}

	filter "configurations:Debug"
		defines "NX_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "NX_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "NX_DIST"
		optimize "On"