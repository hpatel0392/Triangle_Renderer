/* 
* lab5.c
* Author: Harsh Patel
* Spring 2018
* Reads triangle data from a ply file, renders the image and outputs to a ppm file
*/ 
 
#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "vector.h"
#include "lab5.h"


int main(int argc, char* argv[]) {
   
	if (argc != 6) {
		printf("Not enough arguments\nUsage: lab6 <in-file> <out-file> <deg X> <deg Y> <deg Z>\n");
		exit(1);
	}

	// Declare and open img.ppm or writing
	FILE *in = fopen(argv[1], "r");
	FILE *out = fopen(argv[2], "wb");

	if (in == NULL) {
		printf("Error! cannot open %s\n", argv[1]);
		exit(1);
	}

	SCENE_T scene;
	rotateStartPoint(&scene, atoi(argv[3]), atoi(argv[4]), atoi(argv[5]));
	parseFile(in, &scene);

    // loop control
    int x, y, i;

    // Variables operated on, values are updated each loop
	VP_T image = {0};
	double zbuf[Y_RES][X_RES];
	unsigned char img[Y_RES][X_RES] = { 0 };
	double D, n, d;
	VP_T v0, v1, v2, ABC, P;
      
    // ppm header
    fprintf(out, "P5\n%d %d\n255\n", X_RES, Y_RES);
 
    // Start ray tracing 
    for(y = 0; y < Y_RES; y++) {
		for(x = 0; x < X_RES; x++) {
			image = convert(y, x, scene.bounds);
			zbuf[y][x] = HUGE_VAL;
			img[y][x] = 0;
			for (i = 0; i < scene.numFaces; i++) {
				v0 = scene.verts[scene.faces[i].v0];
				v1 = scene.verts[scene.faces[i].v1];
				v2 = scene.verts[scene.faces[i].v2];

				ABC = vec_cross(vec_subtract(v1, v0), vec_subtract(v2, v0));	// eqn 14
				D = vec_dot(vec_scale(ABC, -1), v0);							// eqn 15
				n = vec_dot(vec_scale(ABC, -1), scene.bounds.cam) - D;			// eqn 16
				d = vec_dot(ABC, vec_subtract(image, scene.bounds.cam));		// eqn 17

				if (d <= 0.0001)	// if d close to 0
					continue;
				
																				// eqn 18
				P = vec_add(scene.bounds.cam, vec_scale(vec_subtract(image, scene.bounds.cam), n / d));
				if (intersects(P, v0, v1, v2) && (n/d) < zbuf[y][x]) {
					img[y][x] = 155 + (i % 100);
					zbuf[y][x] = (n / d);
				}
			}
			fwrite(&img[y][x], 1, 1, out);
		}
    }

	free(scene.faces);
	free(scene.verts);

    // closes img.ppm
    fclose(out);
    fclose(in);

    return 0;
}


void rotateStartPoint(SCENE_T * scene, int xi, int yi, int zi){
	double x = (double)xi, y = (double)yi, z = (double)zi;
	const double 
		sinx = sin((x / 360)*(2 * M_PI)),
		cosx = cos((x / 360)*(2 * M_PI)),
		siny = sin((y / 360)*(2 * M_PI)),
		cosy = cos((y / 360)*(2 * M_PI)),
		sinz = sin((z / 360)*(2 * M_PI)),
		cosz = cos((z / 360)*(2 * M_PI));

	MATRIX_T R;
	VP_T cam = (VP_T) { 1, 0, 0 };
	VP_T up = (VP_T) { 0, 0, 1 };

	// rotate about x
	R.r[0] = (VP_T){ 1, 0, 0 };
	R.r[1] = (VP_T){ 0, cosx, -sinx};
	R.r[2] = (VP_T){ 0, sinx, cosx };
	cam = mat_vec_mult(R, cam);
	up = mat_vec_mult(R, up);

	// rotate about y
	R.r[0] = (VP_T){ cosy, 0, siny };
	R.r[1] = (VP_T){ 0, 1, 0 };
	R.r[2] = (VP_T){ -siny, 0, cosy };
	cam = mat_vec_mult(R, cam);
	up = mat_vec_mult(R, up);

	// rotate about z
	R.r[0] = (VP_T){ cosz, -sinz, 0 };
	R.r[1] = (VP_T){ sinz, cosz, 0 };
	R.r[2] = (VP_T){ 0, 0, 1 };
	cam = mat_vec_mult(R, cam);
	up = mat_vec_mult(R, up);

	scene->bounds.cam = cam;
	scene->bounds.up = up;
}


void parseFile(FILE *in, SCENE_T * scene) {
	const char* vertHead = "element vertex";
	const char* faceHead = "element face";
	const char* endHead = "end_header";

	char buf[50] = { '\0' };
	char *start;
	while (fgets(&buf, 50, in) && strstr(buf, endHead) == NULL) {
		start = strstr(buf, vertHead);
		if (start != NULL) {
			start = buf + strlen(vertHead);
			scene->numVerts = atoi(start);
		} else {
			start = strstr(buf, faceHead);
			if (start != NULL) {
				start = buf + strlen(faceHead);
				scene->numFaces = atoi(start);
			}
		}
	}
	
	
	// read in verticies and faces.
	VP_T max, min;
	int i;

	scene->verts = (VP_T*)calloc(scene->numVerts, sizeof(VP_T));
	fscanf(in, "%lf %lf %lf\n", &scene->verts[0].x, &scene->verts[0].y, &scene->verts[0].z);
	vec_copy(&max, scene->verts[0]);
	vec_copy(&min, scene->verts[0]);
	for (i = 1; i < scene->numVerts; i++) {
		fscanf(in, "%lf %lf %lf\n", &scene->verts[i].x, &scene->verts[i].y, &scene->verts[i].z);
		max = vec_elem_max(max, scene->verts[i]);
		min = vec_elem_min(min, scene->verts[i]);
	}

	// read in faces
	int x;
	scene->faces = (TRIANGLE_T*)calloc(scene->numFaces, sizeof(TRIANGLE_T));
	for (i = 0; i < scene->numFaces; i++) {
		fscanf(in, "%d %d %d %d\n", &x, &scene->faces[i].v0, &scene->faces[i].v1, &scene->faces[i].v2);
	}


	// calculate center and E 
	scene->bounds.center = vec_scale(vec_add(max, min), 0.5);
	VP_T temp = vec_subtract(max, min);
	scene->bounds.E = (temp.x > temp.y) ? (temp.x > temp.z ? temp.x : temp.z) : (temp.y > temp.z ? temp.y : temp.z);

	// calculate and set the remaining parameters in BOUNDS_T
	BOUNDS_T * b = &(scene->bounds);
	b->cam = vec_add(vec_scale(b->cam, b->E * 1.5), b->center);			//eqn 4
	b->left = vec_cross(b->up, vec_subtract(b->center, b->cam));		// eqn 5
	double a = vec_len(b->left);										// eqn 6
	b->left = vec_add(vec_scale(b->left,(b->E)/(2*a)), b->center);		// eqn 7
	b->right = vec_cross(vec_subtract(b->center, b->cam), b->up);		// eqn 8
	b->right = vec_add(vec_scale(b->right,(b->E)/(2 * a)), b->center);	// eqn 9
	b->top = vec_add(vec_scale(b->up, b->E / 2), b->center);			// eqn 10
	b->bottom = vec_add(vec_scale(b->up, -(b->E) / 2), b->center);		// eqn 11
	b->topleft = vec_add(vec_scale(b->up, (b->E) / 2), b->left);		// eqn 12

}


/*
* Converts pixel location to image space location
* x = cols, y = rows;
*/
VP_T convert(int x, int y, BOUNDS_T b) {
             
    VP_T dest;
    double COLS = (double) X_RES - 1,
           ROWS = (double) Y_RES - 1; 
	
    vec_copy(&dest, b.topleft);
    dest = vec_add( dest, vec_scale(vec_subtract(b.right, b.left), x / COLS) );
	dest = vec_add( dest, vec_scale(vec_subtract(b.bottom, b.top), y / ROWS) );

    return dest;
}


int intersects(VP_T P, VP_T v0, VP_T v1, VP_T v2) {
	VP_T vec0, vec1, vec2;
	double dot00, dot01, dot02, dot11, dot12, invDenom, u, v;

	vec0 = vec_subtract(v2, v0);
	vec1 = vec_subtract(v1, v0);
	vec2 = vec_subtract(P, v0);

	dot00 = vec_dot(vec0, vec0);
	dot01 = vec_dot(vec0, vec1);
	dot02 = vec_dot(vec0, vec2);
	dot11 = vec_dot(vec1, vec1);
	dot12 = vec_dot(vec1, vec2);

	invDenom = 1 / (dot00 * dot11 - dot01 * dot01);
	u = (dot11 * dot02 - dot01 * dot12) * invDenom;
	v = (dot00 * dot12 - dot01 * dot02) * invDenom;

	return (u >= 0) && (v >= 0) && (u + v < 1);
}
