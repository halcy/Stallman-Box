/**
 * Simple sprite engine code.
 * (c) L. Diener 2012
 */

#include "spriteEngine.h"
#include "vertices.h"
#include "glTools.h"

GLuint spriteVBO;
GLuint spriteEBO;
GLuint spriteProgram;

spriteListEntry spriteList;

void initSpriteEngine() {
	// Buffer objects
	spriteVBO = makeVBO(6, saqVertices);
	spriteEBO = makeEBO(36,sequentialElements);
	
	// Shaders
	GLuint passthroughShader = loadShader("passthrough.vert", GL_VERTEX_SHADER);
	GLuint texturedShader = loadShader("textured.frag", GL_FRAGMENT_SHADER);

	// Shader program
	spriteProgram = makeSimpleProgram(passthroughShader,texturedShader);

	// "Projection" matrix
        matrixAsUniform(idMatrix(), spriteProgram, "projectionMatrix");
	
	// Base entry
	spriteList.next = &spriteList;
	spriteList.prev = &spriteList;
	spriteList.s = NULL;
}

void addSprite(sprite* s) {
	spriteListEntry* tail = spriteList.prev;
	
	spriteListEntry* new = (spriteListEntry*)malloc(sizeof(spriteListEntry));
	new->s = s;
	new->next = &spriteList;
	new->prev = tail;

	tail->next = new;
	spriteList.prev = new;
}

void destroySpriteEngine() {
	spriteListEntry* entry = &spriteList;
	entry = entry->next;
	
	while(entry->s != NULL) {
		spriteListEntry* old = entry;
		entry = entry->next;
		free(old);
	}
}

void drawSingleSprite(sprite* s) {
	glUseProgram(spriteProgram);
	setMvMatrixActive(getSpriteMatrix(s), spriteProgram);
	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D, s->tex );
	glUniform1i(glGetUniformLocation(spriteProgram, "texture1"), 0);
	drawBuffersActive(spriteVBO, spriteEBO, spriteProgram, 6, GL_TRIANGLES);
}

void drawSprites() {
	spriteListEntry* entry = &spriteList;
	entry = entry->next;

	while(entry->s != NULL) {
		drawSingleSprite(entry->s);
		entry = entry->next;
	}
}
