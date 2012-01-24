/**
 * Sprite engine base functions.
 */

#include "gameDraw.h"
#include "glTools.h"
#include "vertices.h"
#include "sprite.h"
#include "spriteEngine.h"
#include "VorbisPlayer.h"
#include "Audio.h"

GLuint saqVBO;
GLuint seqEBO;
GLuint texturedProgram;
GLuint displayProgram;
GLuint spiralA;
GLuint spiralB;
GLuint bgTexture;
GLuint renderFBO;
GLuint renderTexture;

int curWidth;
int curHeight;

float t;

sprite spiralASprite;
sprite spiralBSprite;

VorbisPlayer player;
uint8_t* vorbisdata;

static void GameAudioCallback(void *context,int16_t *samples,int numsamples) {
	ProduceInterleavedSamplesWithVolume((AudioSource *)context,samples,numsamples,0x100);
}


int gameUpdate() {
	t += 0.1;
	spiralASprite.rot = t/20.0;
	return 1;
}

void gameDraw() {
	glViewport(0,0,(GLint)curWidth,(GLint)curHeight);
	glBindFramebuffer(GL_FRAMEBUFFER, renderFBO);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	drawSprites();

	// Postproc from FBO.
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	glUseProgram(displayProgram);
	setMvMatrixActive(rotateMatrix(3.14152,makeVec3(0.0,0.0,1.0)), displayProgram);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, renderTexture );
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, bgTexture );
	glUniform1i(glGetUniformLocation(displayProgram, "texture1"), 0);
	glUniform1i(glGetUniformLocation(displayProgram, "bg"), 1);
	glUniform1f(glGetUniformLocation(displayProgram, "t"), -1.0);
	drawBuffersActive(saqVBO, seqEBO, displayProgram, 6, GL_TRIANGLES);

}

void gameCleanup() {
	destroySpriteEngine();
}

void gameInit() {
	// Sound
	int len;
	InitializeAudio(16000,2,16000/60,false,GameAudioCallback,&player);
	SetAudioGraphSampleRate(ActualAudioSampleRate());
	vorbisdata = loadData("fss.ogg", &len );
	InitializeVorbisPlayer(&player,vorbisdata,len,1);
	StartAudio();

	setupGL();

	prepareVertices();
	
	// Buffer objects
	saqVBO = makeVBO(6, saqVertices);
	seqEBO = makeEBO(36,sequentialElements);
	
	// Shaders
	GLuint passthroughShader = loadShader("passthrough.vert", GL_VERTEX_SHADER);
	GLuint texturedShader = loadShader("textured.frag", GL_FRAGMENT_SHADER);
	GLuint displayShader = loadShader("display.frag", GL_FRAGMENT_SHADER);

	// Shader programs
	texturedProgram = makeSimpleProgram(passthroughShader,texturedShader);
	displayProgram = makeSimpleProgram(passthroughShader,displayShader);

	// Textures
	spiralA = loadTexturePNG("spiral.png");
	spiralB = loadTexturePNG("spiral2.png");
	bgTexture = loadTexturePNG("box.png");
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	
	// FBOs
	renderTexture = makeFloat4Buffer(768,480);
	renderFBO = makeFBO(renderTexture);

	// GL settings
	glClearColor(0.0, 0.0, 0.0, 0.0f);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);

	// Sprite engine
	initSpriteEngine();

	spiralASprite = makeSprite(spiralA,SPRITE_32,0,0,0,1.0,0,0);
	spiralASprite.x = 147;
	spiralASprite.y = -3;
	spiralASprite.w = 355;
	spiralASprite.h = 355;
	addSprite(&spiralASprite);
	
	spiralBSprite = makeSprite(spiralB,SPRITE_32,0,0,0,1.0,1,0);
	spiralBSprite.x = 147;
	spiralBSprite.y = -3;
	spiralBSprite.w = 360;
	spiralBSprite.h = 360;
	addSprite(&spiralBSprite);
	
	// Game
	t = 0;
}

void gameResize(int width, int height) {
	height = height?height:1;
	curWidth = width;
	curHeight = height;
        matrixAsUniform(idMatrix(), texturedProgram, "projectionMatrix");
        matrixAsUniform(idMatrix(), displayProgram, "projectionMatrix");
}