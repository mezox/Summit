#pragma once

#ifndef RENDERER_API
#	ifdef _NOEXPORT
#		define RENDERER_API
#	elif _RENDERER_EXPORT
#		ifdef _WIN32
#			define RENDERER_API __declspec(dllexport)
#		else
#			define RENDERER_API __attribute__((__visibility__("default")))
#		endif
#	else
#		ifdef _WIN32
#			define RENDERER_API __declspec(dllimport)
#		else
#			define RENDERER_API __attribute__((__visibility__("default")))
#		endif
#	endif
#endif
