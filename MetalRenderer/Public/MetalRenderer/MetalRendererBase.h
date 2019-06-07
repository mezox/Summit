#pragma once

#ifndef METALRENDERER_API
#	ifdef _NOEXPORT
#		define METALRENDERER_API
#	elif _METALRENDERER_EXPORT
#		ifdef _WIN32
#			define METALRENDERER_API __declspec(dllexport)
#		else
#			define METALRENDERER_API __attribute__((__visibility__("default")))
#		endif
#	else
#		ifdef _WIN32
#			define METALRENDERER_API __declspec(dllimport)
#		else
#			define METALRENDERER_API __attribute__((__visibility__("default")))
#		endif
#	endif
#endif
