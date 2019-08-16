#pragma once

#ifndef METAL_CPP_API
#	ifdef _NOEXPORT
#		define METAL_CPP_API
#	elif _METAL_CPP_EXPORT
#		ifdef _WIN32
#			define METAL_CPP_API __declspec(dllexport)
#		else
#			define METAL_CPP_API __attribute__((__visibility__("default")))
#		endif
#	else
#		ifdef _WIN32
#			define METAL_CPP_API __declspec(dllimport)
#		else
#			define METAL_CPP_API __attribute__((__visibility__("default")))
#		endif
#	endif
#endif
