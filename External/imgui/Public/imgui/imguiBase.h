#pragma once

#ifndef IMGUI_API
#	ifdef _NOEXPORT
#		define IMGUI_API
#	elif _IMGUI_EXPORT
#		ifdef _WIN32
#			define IMGUI_API __declspec(dllexport)
#		else
#			define IMGUI_API __attribute__((__visibility__("default")))
#		endif
#	else
#		ifdef _WIN32
#			define IMGUI_API __declspec(dllimport)
#		else
#			define IMGUI_API __attribute__((__visibility__("default")))
#		endif
#	endif
#endif
