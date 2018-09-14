/*
* Author: Harsh Patel
* File: vector.c 
* Modified On: 3-2-2018
* Description: Defines functions of the vector type 
*/

#include <math.h>
#include "vector.h"

/*
 * Add two vectors 
 * Hint: return new vector with x1+x2, y1+y2, z1+z2
 */
VP_T vec_add(VP_T v1, VP_T v2)
{
   VP_T result;
   vec_copy(&result, v1);
   
   // component wise addition
   result.x += v2.x;
   result.y += v2.y;
   result.z += v2.z;

   return result; 
}

/*
 * Subtracts two vectors from each other.
 * returns a new vector with new results.
 * Hint: (x1-x2, y1-y2, z1-z2).
 */
VP_T vec_subtract(VP_T v1, VP_T v2)
{
   VP_T result;
    
   // scale v2 by -1 and then add to v1
   v2 = vec_scale(v2, -1.0);
   result = vec_add(v1, v2);

   return result; 
}

/* 
 * Multiply x, y, and z by a constant called 'scale'
 */
VP_T vec_scale(VP_T v, double scale)
{
   VP_T result;
   vec_copy(&result, v);
   
   // scales each component
   result.x *= scale;
   result.y *= scale;
   result.z *= scale;

   return result; 
}

VP_T mat_vec_mult(MATRIX_T m, VP_T v){
	VP_T ret;
	ret.x = vec_dot(m.r[0], v);
	ret.y = vec_dot(m.r[1], v);
	ret.z = vec_dot(m.r[2], v);
	return ret;
}

VP_T vec_elem_max(VP_T v1, VP_T v2)
{
	VP_T ret;
	ret.x = v1.x > v2.x ? v1.x : v2.x;
	ret.y = v1.y > v2.y ? v1.y : v2.y;
	ret.z = v1.z > v2.z ? v1.z : v2.z;
	return ret;
}

VP_T vec_elem_min(VP_T v1, VP_T v2)
{
	VP_T ret;
	ret.x = v1.x < v2.x ? v1.x : v2.x;
	ret.y = v1.y < v2.y ? v1.y : v2.y;
	ret.z = v1.z < v2.z ? v1.z : v2.z;
	return ret;
}

/* 
 * Compute the dot product.
 * Hint: x1*x2, y1*y2, z1*z2, and then sum all the results
 */
double vec_dot(VP_T v1, VP_T v2)
{
   double dot = 0.0;

   // take the dot product
   dot = (v1.x * v2.x ) + (v1.y * v2.y) + (v1.z * v2.z);

   return dot;
}

VP_T vec_cross(VP_T v1, VP_T v2)
{
	VP_T ret;
	ret.x = v1.y*v2.z - v1.z*v2.y;
	ret.y = v1.z*v2.x - v1.x*v2.z;
	ret.z = v1.x*v2.y - v1.y*v2.x;
	return ret;
}

/* 
 * Find the length (magnitude) of the vector.
 */
double vec_len(VP_T v)
{
   double len = 0.0;

   // find magnitude of the vector
   len = sqrt(pow(v.x, 2) + pow(v.y, 2) + pow(v.z, 2));

   return len;
}

/*
* Take each x, y, and z and divide by the length for each
*/
VP_T vec_normalize(VP_T v)
{
   VP_T normal;

   double mag;
   mag = 1.0 / vec_len(v);

   // normalize vector
   normal = vec_scale(v, mag);

   return normal; 
}

/*
 * Copy vector into another.
 * result_v is the output vector.
 */
void vec_copy(VP_T *result_v, VP_T v)
{
   // copy v to result_v
   result_v->x = v.x;
   result_v->y = v.y;
   result_v->z = v.z;
}
