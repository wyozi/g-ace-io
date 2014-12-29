solution "gmsv_gaceio"

	language "C++"
	location ( os.get() .."-".. _ACTION )
	flags { "Symbols", "NoEditAndContinue", "NoPCH", "StaticRuntime", "EnableSSE" }
	targetdir ( "lib/" .. os.get() .. "/" )
	includedirs { "include/", "src/thirdparty/" }

	targetname ("gmsv_gaceio")
	if os.is("windows") then
		targetsuffix ("_win32")
		links {"F:/bootil/lib/windows/vs2010/bootil_static"}
	elseif os.is("linux") then
		targetsuffix ("_linux")
		links {"../bootil/lib/linux/gmake/bootil_static"}
	end

	configurations
	{
		"Release"
	}

	configuration "Release"
		defines { "NDEBUG" }
		flags{ "Optimize", "FloatFast" }

	project "gmsv_gaceio"
		defines { "GMMODULE" }
		files { "src/**.*", "include/**.*" }
		kind "SharedLib"
