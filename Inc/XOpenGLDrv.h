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

// hp2: Harry Potter and the Chamber of Secrets (Unreal Engine 1 build 433) port. Two derived gates:
//  * XOGL_UE1_V4XX — "vanilla UE1 v400-family engine without OldUnreal extensions" (UT99 v400 and HP2
//    433): Core/TCHAR/TArray shims, SDL/WebGL context adoption, gamma defaults. Every former
//    `ENGINE_VERSION==400` adaptation that 433 needs too is keyed on it.
//  * XOGL_HP2 — the 433 render-device face (5-arg DrawComplexSurface, MaxVertices, DrawTriangles) and
//    HP2's blend semantics (Lumos cAlpha, vertex alpha, PF_Highlighted|PF_Translucent straight alpha).
// Both are 0 on every other engine, so those builds are untouched.
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
// ufront: vanilla UT99 v400 port (engine-ut99). v400 ships no separate public "Render.h"; the
// software-renderer internals (FSceneNode/FSurfaceInfo/FSavedPoly/FTransTexture/etc.) live in
// RenderPrivate.h — exactly as the reference NOpenGLESDrv includes it. Gated for a future v200 port.
#elif ENGINE_VERSION==400
#include "RenderPrivate.h"
// hp2: 433 has no RenderPrivate.h; its Render private header is Render/Inc/Render.h (which also pulls
// in Core's UnStat.h). HP2's UnRender.h is include-guarded, so the trailing include below is a no-op.
#elif XOGL_HP2
#include "Render.h"
#endif
// ufront (v400): RenderPrivate.h already pulls in the guard-less UnRender.h, so including it again
// here would redefine URenderDevice/FSceneNode/etc. Skip the trailing include for v400 (the reference
// NOpenGLESDrv includes RenderPrivate.h only, for exactly this reason).
#if ENGINE_VERSION!=400
#include "UnRender.h"
#endif

// ufront: vanilla UT99 v400 compatibility shims — driver-local, gated for a future v200 port. v400
// Core lacks the Windows-style BOOL/TRUE/FALSE the fork uses pervasively, has no PF_None, and no
// NAME_DevGraphics log category. Define them here (after Engine.h/RenderPrivate.h so UBOOL, the PF_
// flags, and the EName enum are already visible). We deliberately do NOT modify the engine.
// hp2: HP2 433 Core/Engine lack the same four names, so the shims apply there too.
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
