#ifndef VERTICES_H
#define VERTICES_H

#include "vectors.h"

#pragma pack(1)
typedef struct vertexentry {
    vec3 position;
    vec3 normal;
    vec2 texcoord;
} vertexentry;

typedef unsigned int elemententry;

extern vertexentry cubeVertices[];
extern vertexentry saqVertices[];

extern elemententry sequentialElements[];
extern elemententry lineElements[];

void prepareVertices();


#endif // VERTICES_H
