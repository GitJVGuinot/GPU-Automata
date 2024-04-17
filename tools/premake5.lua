
-- Conan config
-------------------------------------------------------------------------------
conan = {}
configs = {'Debug','Release','RelWithDebInfo'}
	for i = 1,3 do

		include("../build/deps/"..configs[i].."/conanbuildinfo.premake.lua")
		conan[configs[i]] = {}
		local cfg = conan[configs[i]]
		cfg["build_type"] = conan_build_type
		cfg["arch"] = conan_arch
		cfg["includedirs"] = conan_includedirs
		cfg["libdirs"] = conan_libdirs
		cfg["bindirs"] = conan_bindirs
		cfg["libs"] = conan_libs
		cfg["system_libs"] = conan_system_libs
		cfg["defines"] = conan_defines
		cfg["cxxflags"] = conan_cxxflags
		cfg["cflags"] = conan_cflags
		cfg["sharedlinkflags"] = conan_sharedlinkflags
		cfg["exelinkflags"] = conan_exelinkflags
		cfg["frameworks"] = conan_frameworks

end
-------------------------------------------------------------------------------

-- Conan functions
-------------------------------------------------------------------------------
---------------------------------------
function conan_config_exec()

	configs = {'Debug','Release','RelWithDebInfo'}
	for i = 1,3 do

		local cfg = conan[configs[i]]
		filter("configurations:"..configs[i])

			linkoptions { cfg["exelinkflags"] }
			includedirs{ cfg["includedirs"] }
			libdirs{ cfg["libdirs"] }
			links{ cfg["libs"] }
			links{ cfg["system_libs"] }
			links{ cfg["frameworks"] }
			defines{ cfg["defines"] }
			filter {}
	end
end
---------------------------------------
function conan_config_lib()

	configs = {'Debug','Release','RelWithDebInfo'}
		for i = 1,3 do

			local cfg = conan[configs[i]]
			filter("configurations:"..configs[i])

			linkoptions { cfg["sharedlinkflags"] }
			includedirs{ cfg["includedirs"] }
			defines{ cfg["defines"] }
			filter {}
	end
end
---------------------------------------
-------------------------------------------------------------------------------

-- Global
-------------------------------------------------------------------------------
workspace "Main"

	configurations { "Debug", "Release"}
	architecture "x64"
	location "../build/Visual_Studio"
	cppdialect "c++20"
	startproject "Test"

	filter "configurations:Debug"
	defines { "DEBUG" }
	symbols "On"
	runtime "Debug"

	filter "configurations:Release"
	defines { "NDEBUG" }
	optimize "On"
	runtime "Release"

filter {}
-------------------------------------------------------------------------------

-- Test
-------------------------------------------------------------------------------
project "Test"

kind "ConsoleApp"
language "C++"
targetdir "../build/%{prj.name}/%{cfg.buildcfg}"
includedirs { "../include", "../deps/include" }
filter "configurations:Debug"
  links {"../deps/libs/jam_engine/JAM_Engine_x64_d.lib"}
filter "configurations:Release"
  links {"../deps/libs/jam_engine/JAM_Engine_x64.lib"}
conan_config_exec()
debugargs { _MAIN_SCRIPT_DIR .. "/examples/data" }
files {
  "../assets/**",
  "../deps/include/**",
  "../deps/src/**",
  "../include/**",
  "../src/**",
}
filter "files:**.obj"
    flags { "ExcludeFromBuild" }
-------------------------------------------------------------------------------
