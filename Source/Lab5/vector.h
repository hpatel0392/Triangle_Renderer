/*
* Author: Harsh Patel
* File: vector.h
* Modified On: 3/2/2018	
* Description: This header file contains vector definitions and
*              prototype statements for 3-D vector operations   
*/

#ifndef VECTOR_H
#define VECTOR_H

// Structure definition for a vector
typedef struct {
   double x;
   double y;
   double z;
} VP_T;

typedef struct {
	VP_T r[3];
} MATRIX_T;

/* Prototype Statements */
VP_T vec_add(VP_T  v1, VP_T  v2);
VP_T vec_subtract(VP_T v1, VP_T v2);
VP_T vec_scale(VP_T v, double scale);
VP_T mat_vec_mult(MATRIX_T m, VP_T v);
VP_T vec_elem_max(VP_T v1, VP_T v2);
VP_T vec_elem_min(VP_T v1, VP_T v2);
double vec_dot(VP_T v1, VP_T v2);
VP_T vec_cross(VP_T v1, VP_T v2);
double vec_len(VP_T v);
VP_T vec_normalize(VP_T v);


void vec_copy(VP_T *result_v, VP_T v);

#endif
