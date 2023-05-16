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

	if os.target() == "windows" then
		defines 
		{ 
			"MAPP_CUSTOM_PLATFORM_DETECTION", 
			"MAPP_PLATFORM_WIN32", 
			"TEST_LINKING" 
		}
		links 
		{ 
			"Xinput9_1_0" 
		}

		printf("Completed MAPP [Windows]")

	elseif os.target() == "macosx" then
		defines 
		{ 
			"MAPP_CUSTOM_PLATFORM_DETECTION", 
			"MAPP_PLATFORM_COCOA", 
			"TEST_LINKING" 
		}
		links 
		{ 
			"Cocoa.framework", 
			"QuartzCore.framework", 
			"Metal.framework", 
			 -- "IOKit.framework", 
			 -- "CoreVideo.framework" 
		}
		buildoptions {"-F /Library/Frameworks", "-F ~/Library/Frameworks"}
        linkoptions {"-F /Library/Frameworks", "-F ~/Library/Frameworks"}

		printf("Completed MAPP [MacOSX]")

	else
		printf("Unable to find os.target()")
	end