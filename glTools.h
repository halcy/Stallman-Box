#ifndef __GLTOOLS_H__
#define __GLTOOLS_H__

/**
 * A bunch of low level opengl-related things that are better off wrapped.
 * Mostly concerning the generation and use of Buffer Objects, Shaders and
 * Textures.
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "vertices.h"
#include "matrices.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

uint8_t* loadData(const char* filename, int* len);
char* loadText(const char* filename, int* len);

GLuint loadShader(const char* filename, GLuint type);
GLuint makeSimpleProgram(GLuint vert, GLuint frag);

GLuint makeFloat4Buffer(int w, int h);
GLuint makeIntBuffer(int w, int h);

GLuint makeFBO(GLuint texture);
GLuint makeVBO(int entries, vertexentry* data);
GLuint makeEBO(int entries, elemententry* data);

void drawBuffers(GLuint vbo, GLuint ebo, GLuint shader, int entries, GLenum type);
void setMvMatrix(matrix mvMatrixO, GLuint program);

void drawBuffersActive(GLuint vbo, GLuint ebo, GLuint shader, int entries, GLenum type);
void setMvMatrixActive(matrix mvMatrixO, GLuint program);

void bindShaderWithLocations(GLuint shader);

void setupGL();

GLuint loadTextureTGA(const char *filename);
GLuint loadTexturePNG(const char *filename);

#endif