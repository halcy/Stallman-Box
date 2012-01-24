#ifndef __SPRITE_H__
#define __SPRITE_H__

/**
 * Single sprite managment structure and related functions.
 * (c) L. Diener 2012
 */

#include "glTools.h"

#define SPRITE_8 0
#define SPRITE_16 1
#define SPRITE_32 3
#define SPRITE_64 4

typedef struct {
	float x;
	float y;
	float rot;

	float scale;

	bool hflip;
	bool vflip;
	
	GLuint tex;
	
	float w;
	float h;
} sprite;

sprite makeSprite(GLuint tex, int type, float x, float y, float rot, float scale, int hflip, int vflip);
matrix getSpriteMatrix(sprite* s);

#endif