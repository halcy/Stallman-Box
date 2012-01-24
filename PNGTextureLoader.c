/**
 * Functions to load PNG data to a format suitable for OpenGL textures.
 * (c) 2012 L. Diener
 */

#include "PNGTextureLoader.h"

#include "glTools.h"
#include "PNG.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

static void* floatPremultAlphaPixelFunc(void *destination, uint8_t r, uint8_t g, uint8_t b, uint8_t a, int x, int y) {
	x=y;
	float *pixel = destination;
	pixel[0] = (((float)r)/255.0)*(((float)a)/255.0);
	pixel[1] = (((float)g)/255.0)*(((float)a)/255.0);
	pixel[2] = (((float)b)/255.0)*(((float)a)/255.0);
	pixel[3] = (((float)a)/255.0);
	return(pixel+4);
}

float* loadPNGToFloatBuffer(const char* filename, int* width, int* height) {
	int len = 0;
	uint8_t* data = loadData(filename, &len);
	
	PNGLoader p;
	InitializePNGLoader(&p, data, len);
	
	if(!LoadPNGHeader(&p)) {
		fprintf(stderr, "ERROR: PNG parsing error.\n");
		free(data);
		return NULL;
	}

	float* pixels = (float*)malloc(p.width*p.height*4*sizeof(float));
	if(!LoadPNGImageData(&p, pixels, p.width*4*sizeof(float), &floatPremultAlphaPixelFunc)) {
		fprintf(stderr, "ERROR: PNG loading error.\n");
		free(pixels);
		free(data);
		return NULL;
	}

	*width = p.width;
	*height = p.height;

	free(data);
	return pixels;
}
