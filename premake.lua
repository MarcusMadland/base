project "mapp"
	kind "StaticLib"
	language "C++"
	cppdialect "C++14"
	exceptionhandling "Off"
	rtti "Off"
	defines "__STDC_FORMAT_MACROS"

	files
	{
		"include/**.hpp",
		"src/**.mm",
		"src/*.m",
		"src/**.cpp",
	}

	includedirs
	{
		"include",
	}

	filter "system:windows"
		defines 
		{ 
			"MAPP_CUSTOM_PLATFORM_DETECTION", 
			"MAPP_PLATFORM_WIN32",
		}
		links 
		{ 
			"Xinput9_1_0", 
			"gdi32", 
			"kernel32", 
			"psapi",
		}
	filter ""

	filter "system:macosx"
		defines 
		{ 
			"MAPP_CUSTOM_PLATFORM_DETECTION", 
			"MAPP_PLATFORM_COCOA",
		}
		links 
		{ 
			"Cocoa.framework", 
			"QuartzCore.framework", 
			"Metal.framework",
		}
		buildoptions 
		{ 
			"-x objective-c++", 
		}
	filter ""