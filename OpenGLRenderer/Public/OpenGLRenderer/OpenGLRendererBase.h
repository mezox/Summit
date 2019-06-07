#pragma once

#ifndef OPENGLRENDERER_API
#	ifdef _NOEXPORT
#		define OPENGLRENDERER_API
#	elif _OPENGLRENDERER_EXPORT
#		ifdef _WIN32
#			define OPENGLRENDERER_API __declspec(dllexport)
#		else
#			define OPENGLRENDERER_API __attribute__((__visibility__("default")))
#		endif
#	else
#		ifdef _WIN32
#			define OPENGLRENDERER_API __declspec(dllimport)
#		else
#			define OPENGLRENDERER_API __attribute__((__visibility__("default")))
#		endif
#	endif
#endif
