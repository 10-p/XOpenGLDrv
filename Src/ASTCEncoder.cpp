/*=============================================================================
	ASTCEncoder.cpp: ARM astc-encoder on several threads, for the DXT1 transcode
	(SetTexture.cpp, XOPENGL_TRANSCODE builds). Kept apart from the engine headers,
	whose macros the standard thread headers must not meet.
=============================================================================*/

#if XOPENGL_TRANSCODE
#include <stddef.h>
#include <stdint.h>
#include <thread>
#include <vector>

#include "astcenc.h"

// The encoder's threads: one a core, up to 8 (one where the build has no threads).
unsigned int XOpenGLASTCThreads()
{
#if defined(__EMSCRIPTEN__) && !defined(__EMSCRIPTEN_PTHREADS__)
	return 1;
#else
	const unsigned int Count = std::thread::hardware_concurrency();
	return Count < 1 ? 1 : (Count > 8 ? 8 : Count);
#endif
}

// Encodes Width x Height RGBA texels (whole 4x4 blocks) to ASTC on a context made for XOpenGLASTCThreads()
// threads; Size bytes at Out. The context shares an image's blocks among the threads that call it and any of them
// can finish it alone, so a small image is encoded on the calling thread only. True on success.
bool XOpenGLEncodeASTC(astcenc_context* Context, void* Pixels, unsigned int Width, unsigned int Height, uint8_t* Out, size_t Size)
{
	astcenc_image Image = { Width, Height, 1, ASTCENC_TYPE_U8, &Pixels };
	const astcenc_swizzle Swizzle = { ASTCENC_SWZ_R, ASTCENC_SWZ_G, ASTCENC_SWZ_B, ASTCENC_SWZ_A };
	const unsigned int Threads = Width * Height >= 256 * 256 ? XOpenGLASTCThreads() : 1;
	std::vector<astcenc_error> Errors(Threads, ASTCENC_SUCCESS);
	std::vector<std::thread> Workers;
	for (unsigned int i = 1; i < Threads; i++)
		Workers.emplace_back([&, i]() { Errors[i] = astcenc_compress_image(Context, &Image, &Swizzle, Out, Size, i); });
	Errors[0] = astcenc_compress_image(Context, &Image, &Swizzle, Out, Size, 0);
	for (std::thread& Worker : Workers)
		Worker.join();
	astcenc_compress_reset(Context);
	for (astcenc_error Error : Errors)
		if (Error != ASTCENC_SUCCESS)
			return false;
	return true;
}
#endif
