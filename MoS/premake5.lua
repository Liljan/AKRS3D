workspace "GL"
	configurations { "Debug", "Release" }

	--platforms {"Win64"}
	architecture  "x86_64"
	language "C++"
	cppdialect "C++17"

	outputdir = "build/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
	--projectsdir = "projects/"

-- The Project

project "RB"

	location (projectsdir)

	-- Todo: What kinds of "kind":s are there?
	kind "ConsoleApp"
	
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")

	includedirs
	{
		"src/",

		"external/glm/",
		"external/glfw/include/",
		"external/glad/include/",
		"external/glew/include/"
	}

	-- Project-specific includes
	files
	{
		"src/**.h",
		"src/**.cpp",
	}

	-- Project-specific includes (e.g. Test lib includes)
	--includedirs {"src/%{prj.name}/"}

	libdirs {"external/glfw/lib-vc2019/", "external/glad/lib/", "external/glew/lib/Release/x64/"}

	links { "opengl32.lib", "glfw3.lib", "GLAD.lib", "glew32.lib" }

	filter "system:windows"
    	systemversion "latest"
  		defines { "GAME_PLATFORM_WINDOWS" }

	filter "configurations:Debug"
		-- Defines a flag
		defines {"DEBUG"}
		symbols "On"

		--links {  }

	filter "configurations:Release"
		defines {"NDEBUG"}
		optimize "On"

		--links { "sfml-audio-s.lib", "sfml-graphics-s.lib", "sfml-network-s.lib", "sfml-system-s.lib", "sfml-window-s.lib", "openal32.dll" }
