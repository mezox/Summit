#pragma once

#ifndef METAL_RENDERER_API
#	ifdef _NOEXPORT
#		define METAL_CPP_API
#	elif _METAL_RENDERER_EXPORT
#		ifdef _WIN32
#			define METAL_RENDERER_API __declspec(dllexport)
#		else
#			define METAL_RENDERER_API __attribute__((__visibility__("default")))
#		endif
#	else
#		ifdef _WIN32
#			define METAL_RENDERER_API __declspec(dllimport)
#		else
#			define METAL_RENDERER_API __attribute__((__visibility__("default")))
#		endif
#	endif
#endif
