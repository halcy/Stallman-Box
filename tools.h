#ifndef TOOLS_H
#define TOOLS_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>

#include "itoa.h"

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GL/freeglut.h>
#else
#include <GL/glew.h>
#include <GL/freeglut.h>
#endif

#define PI 3.14159265
#define deg2rad(x) ((PI*(x))/(180.0))

#define bool uint8_t
#define true 1
#define false 0


// Global status things.
extern int mouseX;
extern int mouseY;
extern int key[256];
extern int specialKey[256];


#endif
