project "mapp"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	exceptionhandling "Off"
	rtti "Off"

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
		defines { "MAPP_PLATFORM_WIN32" }
		links { "Xinput9_1_0" }
	filter "system:macosx"
		defines { "MAPP_PLATFORM_COCOA" }
		links { "Cocoa.framework", "QuartzCore.framework", "Metal.framework", 
			 -- "IOKit.framework", "CoreVideo.framework" 
		}