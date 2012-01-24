#ifndef __PNG_H__
#define __PNG_H__

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct PNGLoader
{
	const uint8_t *bytes;
	size_t length;

	int width,height;
	int bitdepth,colourtype;
	int bytesperrow,bytesperpixel;

	const uint8_t *palettestart;
	int palettecount;

	const uint8_t *transparencystart;
	int transparencycount;

	const uint8_t *idatstart;
	uint32_t idatsize;
} PNGLoader;

typedef void *PNGPixelFunction(void *destination,uint8_t r,uint8_t g,uint8_t b,uint8_t a,int x,int y);

void InitializePNGLoader(PNGLoader *self,const void *bytes,size_t length);
bool LoadPNGHeader(PNGLoader *self);
bool LoadPNGImageData(PNGLoader *self,void *pixels,int bytesperrow,PNGPixelFunction *pixelfunc);
bool LoadPNGImageDataWithTemporaryStorage(PNGLoader *self,void *pixels,int bytesperrow,PNGPixelFunction *pixelfunc,void *temporarystorage);
size_t TemporaryBytesForPNGLoader(PNGLoader *self);

#endif

