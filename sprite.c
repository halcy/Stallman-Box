/**
 * Single sprite managment structure and related functions.
 * (c) L. Diener 2012
 */

#include "sprite.h"

sprite makeSprite(GLuint tex, int type, float x, float y, float rot, float scale, int hflip, int vflip) {
	sprite s;

	s.x = x;
	s.y = y;
	s.rot = rot;

	s.scale = scale;
	s.hflip = hflip;
	s.vflip = vflip;
	
	s.tex = tex;
	
	if(type == SPRITE_8) {
		s.w = s.h = 8.0;
	}
	else if(type == SPRITE_16) {
		s.w = s.h = 16.0;
	}
	else if(type == SPRITE_32) {
		s.w = s.h = 32.0;
	}
	else if(type == SPRITE_64) {
		s.w = s.h = 64.0;
	}
	else {
		s.w = s.h = 8.0;
	}

	return s;
}

matrix getSpriteMatrix(sprite* s) {
 	matrix spriteMatrix = idMatrix();

	matrixCombine(&spriteMatrix,translateMatrix((2.0/768.0)*s->x,(2.0/480.0)*s->y,0.0));
	matrixCombine(&spriteMatrix,scaleMatrix((1.0/768.0)*s->w,(1.0/480.0)*s->h,1.0));
	matrixCombine(&spriteMatrix,rotateMatrix(s->rot,makeVec3(0.0,0.0,1.0)));
	matrixCombine(&spriteMatrix,scaleMatrix(s->scale,s->scale,1.0));
	matrixCombine(&spriteMatrix,scaleMatrix(s->hflip?-1.0:1.0,s->vflip?-1.0:1.0,1.0));
	
	return spriteMatrix;
}
