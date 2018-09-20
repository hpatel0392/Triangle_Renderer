/*
* lab5.h
* Author: Harsh Patel
* Spring 2018
*/

#ifndef LAB5_H
#define LAB5_H

#include <stdio.h>
#include <string.h>
#include "vector.h"

#define X_RES 256
#define Y_RES 256

// structure definitions

// indicies to the
typedef struct{
	int v0, v1, v2;
} TRIANGLE_T;

typedef struct {
	VP_T cam;
	VP_T  up;
	double E;
	VP_T center;
	VP_T left;
	VP_T right;
	VP_T top;
	VP_T bottom;
	VP_T topleft;
} BOUNDS_T;

typedef struct {
	int numVerts;
	int numFaces;
	VP_T * verts;
	TRIANGLE_T * faces;
	BOUNDS_T bounds;
} SCENE_T;

void rotateStartPoint(SCENE_T *, int, int, int);
void parseFile(FILE *, SCENE_T *);
VP_T convert(int, int, BOUNDS_T);
int intersects(VP_T, VP_T, VP_T, VP_T);
#endif
