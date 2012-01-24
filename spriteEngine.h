#ifndef __SPRITE_ENGINE_H__
#define __SPRITE_ENGINE_H__

#include "sprite.h"

typedef struct spriteListEntry {
	sprite* s;
	struct spriteListEntry* next;
	struct spriteListEntry* prev;
	
} spriteListEntry;


void initSpriteEngine();
void addSprite(sprite* s);
void destroySpriteEngine();
void drawSingleSprite(sprite* s);
void drawSprites();

#endif
