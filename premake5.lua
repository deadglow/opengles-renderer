local mergeTables = function(into, from)
	for _,v in ipairs(from) do
		table.insert(into, v)
	end

	return into
end

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

newoption {
	trigger = "physics",
	value = "backend",
	description =  "Choose a backend for the physics engine.",
	allowed = {
		{ "none", "No physics engine..."},
		{ "jolt", "Jolt physics"},
	},
	default = "jolt",
}

local physTable = {
	["none"] = "PHYSICS_NONE",
	["jolt"] = "PHYSICS_JOLT",
}


-- premake5.lua
workspace("opengles-renderer")
	configurations( { "debug", "release" } )
	location("build")

local projectName = "opengles-renderer"
project(projectName)
	kind("ConsoleApp")
	language("C++")
	targetdir("bin/%{cfg.buildcfg}")
	location("build/" .. projectName)

	files( {"src/*.h", "src/*.c", "src/*.hpp", "src/*.cpp" } )
	includedirs( { "include" } )

	libdirs( {"lib" } )
	links( {"mingw32", "SDL2main", "SDL2", "EGL", "GLESv2", "Jolt" } )
	
	-- filter("action:gmake2")
	-- 	links( { "mingw32" })
	-- filter({ })

	-- physics implementation
	defines( { physTable[_OPTIONS["physics"]] })
	defines( { "ENABLE_DEBUG_RENDER", "JPH_DEBUG_RENDERER", "JPH_PROFILE_ENABLED" } )

	-- filter("options:not nodebugrender")

	-- configurations
	filter("configurations:debug")
		defines( { "DEBUG", "JPH_ENABLE_ASSERTS" })
		symbols("On")

	filter("configurations:release")
		defines( { "NDEBUG" })
		optimize("Speed")

	flags( { "Verbose" } )

