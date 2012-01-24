#ifndef __PNG_TEXTURE_LOADER_H__
#define __PNG_TEXTURE_LOADER_H__

/**
 * Functions to load PNG data to a format suitable for OpenGL textures.
 * (c) 2012 L. Diener
 */

float* loadPNGToFloatBuffer(const char* filename, int* width, int* height);

#endif
