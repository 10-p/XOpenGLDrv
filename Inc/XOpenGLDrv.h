/*=============================================================================
	XOpenGlDrv.h: Unreal OpenGL support header.
	Copyright 2014-2016 Oldunreal

	Revision history:
		* Created by Smirftsch

=============================================================================*/

/*-----------------------------------------------------------------------------
	Includes.
-----------------------------------------------------------------------------*/

#ifndef _INCL_XOPENGLDRV_H_
#define _INCL_XOPENGLDRV_H_

#ifdef WIN32
#include <windows.h>
#endif
#include <cmath>
#ifndef WIN32
using std::isnan;
using std::isfinite;
#endif
#include "Engine.h"

// Engine gates, both 0 on every other engine:
//  * XOGL_UE1_V4XX: a UE1 v400-family engine without the OldUnreal extensions (UT99 v400, HP2 433):
//    Core/TCHAR/TArray shims, the windowing driver's GL context, gamma defaults.
//  * XOGL_HP2: Harry Potter and the Chamber of Secrets (UE1 build 433): its render-device interface
//    (5-argument DrawComplexSurface, MaxVertices, DrawTriangles) and its blend semantics (Lumos
//    cAlpha, vertex alpha, PF_Highlighted|PF_Translucent straight alpha).
#if ENGINE_VERSION==400 || ENGINE_VERSION==433
#define XOGL_UE1_V4XX 1
#else
#define XOGL_UE1_V4XX 0
#endif
#if ENGINE_VERSION==433
#define XOGL_HP2 1
#else
#define XOGL_HP2 0
#endif

#if ENGINE_VERSION==227
#include "Render.h"
#elif ENGINE_VERSION==430
#include "Render.h"
#elif ENGINE_VERSION>=436 && ENGINE_VERSION<1100
#include "Render.h"
#elif ENGINE_VERSION==1100
#include "RenderPrivate.h"
// UE1 v400 has no public Render.h; FSceneNode, FSurfaceInfo, FSavedPoly, FTransTexture and the other
// renderer types are in RenderPrivate.h.
#elif ENGINE_VERSION==400
#include "RenderPrivate.h"
// 433 has no RenderPrivate.h; its renderer header is Render.h (which also includes Core's UnStat.h).
// 433's UnRender.h is include-guarded, so the include below is harmless.
#elif XOGL_HP2
#include "Render.h"
#endif
// v400's RenderPrivate.h already includes UnRender.h, which has no include guard.
#if ENGINE_VERSION!=400
#include "UnRender.h"
#endif

// v400 Core has no BOOL/TRUE/FALSE, no PF_None and no NAME_DevGraphics log category. They are defined
// here, after Engine.h and RenderPrivate.h, so UBOOL, the PF_ flags and EName are visible.
// HP2 433 lacks the same names.
#if XOGL_UE1_V4XX
typedef UBOOL BOOL;
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif
#ifndef PF_None
#define PF_None 0
#endif
#ifndef NAME_DevGraphics
#define NAME_DevGraphics NAME_Init
#endif
#endif

#if !defined(SDL2BUILD) && !defined(SDL3BUILD)
# ifdef __LINUX__
#  define SDLBUILD  1
#  define SDL2BUILD 1
#  define SDL3BUILD 0
# elif MACOSX
#  define SDLBUILD  1
#  define SDL2BUILD 0
#  define SDL3BUILD 1
# elif defined(__EMSCRIPTEN__)
#  define SDLBUILD  1
#  define SDL2BUILD 0
#  define SDL3BUILD 1
# endif
#endif

//#define AUTO_INITIALIZE_REGISTRANTS_OPENGLDRV UXOpenGLRenderDevice::StaticClass();
extern "C" { void autoInitializeRegistrantsXOpenGLDrv(void); }
#define AUTO_INITIALIZE_REGISTRANTS_XOPENGLDRV autoInitializeRegistrantsXOpenGLDrv();

#endif
/*-----------------------------------------------------------------------------
	The End.
-----------------------------------------------------------------------------*/
