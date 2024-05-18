-- premake5.lua
workspace "opengles-renderer"
	configurations { "debug", "release" }
	location "build"

local projectName = "opengles-renderer"
project(projectName)
	kind "ConsoleApp"
	language "C++"
	targetdir "bin/%{cfg.buildcfg}"
	location("build/" .. projectName)

	files { "src/*.h", "src/*.hpp", "src/*.c", "src/*.cpp" }
	includedirs { "include" }

	libdirs { "lib" }
	links { "mingw32", "SDL2main", "SDL2", "EGL", "GLESv2" }

	filter "configurations:debug"
		defines { "DEBUG" }
		symbols "On"

	filter "configurations:release"
		defines { "NDEBUG" }
		optimize "Speed"

		

-- Clean Function --
newaction {
	trigger     = "clean",
	description = "clean the software",
	execute     = function ()
	   print("clean the build...")
	   os.rmdir("./build")
	   print("done.")
	end
 }