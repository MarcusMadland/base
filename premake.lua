project "mapp"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir ("binaries/" .. "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}" .. "/%{prj.name}")
	objdir ("intermediate/" .. "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}" .. "/%{prj.name}")

	files
	{
		"include/**.hpp",
		"src/**.mm",
		"src/*.m",
		"src/**.cpp",
	}

	includedirs
	{
		"include"
	}

	defines { "MAPP_CUSTOM_PLATFORM_DETECTION"}

	filter "system:windows"
		defines { "MAPP_WIN32" }
	filter "system:macosx"
		defines { "MAPP_COCOA" }
	