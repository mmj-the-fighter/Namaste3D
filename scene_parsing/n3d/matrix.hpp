#ifndef _N3D_MATRIX_HPP_
#define _N3D_MATRIX_HPP_

#include "vector.hpp"
#include "plane.hpp"

namespace n3d{
#define TORADIANS(x)	((x)*0.0174532925f)
#define TODEGREES(x)	((x)*57.295779513f)

#define M4x4_11 0
#define M4x4_21 1
#define M4x4_31 2
#define M4x4_41 3

#define M4x4_12 4
#define M4x4_22 5
#define M4x4_32 6
#define M4x4_42 7

#define M4x4_13 8
#define M4x4_23 9
#define M4x4_33 10
#define M4x4_43 11

#define M4x4_14 12
#define M4x4_24 13
#define M4x4_34 14
#define M4x4_44 15



	//4x4 Column major order reference:
	//|  0  4  8 12 |
	//|  1  5  9 13 |
	//|  2  6 10 14 |
	//|  3  7 11 15 |

	typedef float mat4x4[16];
	typedef float mat3x3[9];

	//3x3 Column major order reference:
	// 0 3 6 
	// 1 4 7
	// 2 5 8


#define M3x3_11 0
#define M3x3_21 1
#define M3x3_31 2

#define M3x3_12 3
#define M3x3_22 4
#define M3x3_32 5

#define M3x3_13 6
#define M3x3_23 7
#define M3x3_33 8




	inline void mat4x4_set_zero(mat4x4 m)
	{
		m[0] = 0.0f;
		m[1] = 0.0f;
		m[2] = 0.0f;
		m[3] = 0.0f;
		m[4] = 0.0f;
		m[5] = 0.0f;
		m[6] = 0.0f;
		m[7] = 0.0f;
		m[8] = 0.0f;
		m[9] = 0.0f;
		m[10] = 0.0f;
		m[11] = 0.0f;
		m[12] = 0.0f;
		m[13] = 0.0f;
		m[14] = 0.0f;
		m[15] = 0.0f;
	}

	inline void mat4x4_set_identity(mat4x4 m)
	{
		m[0] = 1.0f;
		m[1] = 0.0f;
		m[2] = 0.0f;
		m[3] = 0.0f;
		m[4] = 0.0f;
		m[5] = 1.0f;
		m[6] = 0.0f;
		m[7] = 0.0f;
		m[8] = 0.0f;
		m[9] = 0.0f;
		m[10] = 1.0f;
		m[11] = 0.0f;
		m[12] = 0.0f;
		m[13] = 0.0f;
		m[14] = 0.0f;
		m[15] = 1.0f;
	}

	inline void mat4x4_extract_upper_left_3x3(mat3x3 result, mat4x4 m) {
		result[M3x3_11] = m[M4x4_11];
		result[M3x3_21] = m[M4x4_21];
		result[M3x3_31] = m[M4x4_31];
		result[M3x3_12] = m[M4x4_12];
		result[M3x3_22] = m[M4x4_22];
		result[M3x3_32] = m[M4x4_32];
		result[M3x3_13] = m[M4x4_13];
		result[M3x3_23] = m[M4x4_23];
		result[M3x3_33] = m[M4x4_33];
	}

	inline void mat4x4_set_translation(mat4x4 m, float tx, float ty, float tz)
	{
		m[0] = 1.0f;
		m[1] = 0.0f;
		m[2] = 0.0f;
		m[3] = 0.0f;
		m[4] = 0.0f;
		m[5] = 1.0f;
		m[6] = 0.0f;
		m[7] = 0.0f;
		m[8] = 0.0f;
		m[9] = 0.0f;
		m[10] = 1.0f;
		m[11] = 0.0;
		m[12] = tx;
		m[13] = ty;
		m[14] = tz;
		m[15] = 1.0f;
	}

	inline void mat3x3_transpose(mat3x3 result, mat3x3 m)
	{
		result[0] = m[0];
		result[1] = m[3];
		result[2] = m[6];
		result[3] = m[1];
		result[4] = m[4];
		result[5] = m[7];
		result[6] = m[2];
		result[7] = m[5];
		result[8] = m[8];
	}

	inline void mat3x3_multiply_vec3f(vec3f* result, mat3x3 m, vec3f *v)
	{
		result->x = m[0] * v->x + m[3] * v->y + m[6] * v->z;
		result->y = m[1] * v->x + m[4] * v->y + m[7] * v->z;
		result->z = m[2] * v->x + m[5] * v->y + m[8] * v->z;
	}


	inline float mat3x3_determinant(mat3x3 m)
	{
		return m[0] * (m[4] * m[8] - m[5] * m[7])
			- m[3] * (m[1] * m[8] - m[2] * m[7])
			+ m[6] * (m[1] * m[5] - m[2] * m[4]);
	}

	inline int mat3x3_inverse(mat3x3 result, mat3x3 m)
	{
		mat3x3 tmp;
		float det = mat3x3_determinant(m);
		if (det == 0.0f) {
			return 1;
		}
		float oneOverDet = 1.0f / det;
		tmp[0] = oneOverDet * (m[4] * m[8] - m[5] * m[7]);
		tmp[1] = oneOverDet * (m[2] * m[7] - m[1] * m[8]);
		tmp[2] = oneOverDet * (m[1] * m[5] - m[2] * m[4]);
		tmp[3] = oneOverDet * (m[5] * m[6] - m[3] * m[8]);
		tmp[4] = oneOverDet * (m[0] * m[8] - m[2] * m[6]);
		tmp[5] = oneOverDet * (m[2] * m[3] - m[0] * m[5]);
		tmp[6] = oneOverDet * (m[3] * m[7] - m[4] * m[6]);
		tmp[7] = oneOverDet * (m[1] * m[6] - m[0] * m[7]);
		tmp[8] = oneOverDet * (m[0] * m[4] - m[1] * m[3]);

		result[0] = tmp[0];
		result[1] = tmp[1];
		result[2] = tmp[2];
		result[3] = tmp[3];
		result[4] = tmp[4];
		result[5] = tmp[5];
		result[6] = tmp[6];
		result[7] = tmp[7];
		result[8] = tmp[8];

		return 0;
	}

	inline void mat4x4_set_roation_x(mat4x4 m, float angle_in_degrees)
	{
		float r = TORADIANS(angle_in_degrees);
		float sin_r = sin(r);
		float cos_r = cos(r);
		m[0] = 1.0f;
		m[1] = 0.0f;
		m[2] = 0.0f;
		m[3] = 0.0f;
		m[4] = 0.0f;
		m[5] = cos_r;
		m[6] = -sin_r;
		m[7] = 0.0f;
		m[8] = 0.0f;
		m[9] = sin_r;
		m[10] = cos_r;
		m[11] = 0.0f;
		m[12] = 0.0f;
		m[13] = 0.0f;
		m[14] = 0.0f;
		m[15] = 1.0f;
	}

	inline void mat4x4_set_roation_y(mat4x4 m, float angle_in_degrees)
	{
		float r = TORADIANS(angle_in_degrees);
		float sin_r = sin(r);
		float cos_r = cos(r);
		m[0] = cos_r;
		m[1] = 0.0f;
		m[2] = sin_r;
		m[3] = 0.0f;
		m[4] = 0.0f;
		m[5] = 1.0f;
		m[6] = 0.0f;
		m[7] = 0.0f;
		m[8] = -sin_r;
		m[9] = 0.0f;
		m[10] = cos_r;
		m[11] = 0.0f;
		m[12] = 0.0f;
		m[13] = 0.0f;
		m[14] = 0.0f;
		m[15] = 1.0f;
	}

	inline void mat4x4_set_roation_z(mat4x4 m, float angle_in_degrees)
	{
		float r = TORADIANS(angle_in_degrees);
		float sin_r = sin(r);
		float cos_r = cos(r);
		m[0] = cos_r;
		m[1] = -sin_r;
		m[2] = 0.0f;
		m[3] = 0.0f;
		m[4] = sin_r;
		m[5] = cos_r;
		m[6] = 0.0f;
		m[7] = 0.0f;
		m[8] = 0.0f;
		m[9] = 0.0f;
		m[10] = 1.0f;
		m[11] = 0.0f;
		m[12] = 0.0f;
		m[13] = 0.0f;
		m[14] = 0.0f;
		m[15] = 1.0f;
	}

	inline void mat4x4_set_scale(mat4x4 m, float sx, float sy, float sz)
	{
		m[0] = sx;
		m[1] = 0.0f;
		m[2] = 0.0f;
		m[3] = 0.0f;
		m[4] = 0.0f;
		m[5] = sy;
		m[6] = 0.0f;
		m[7] = 0.0f;
		m[8] = 0.0f;
		m[9] = 0.0f;
		m[10] = sz;
		m[11] = 0.0f;
		m[12] = 0.0f;
		m[13] = 0.0f;
		m[14] = 0.0f;
		m[15] = 1.0f;
	}

	inline void mat4x4_set_ortho(mat4x4 m, float left, float right, float bottom, float top, float near, float far)
	{
		m[0] = 2.0f / (right - left);
		m[1] = 0.0f;
		m[2] = 0.0f;
		m[3] = 0.0f;
		m[4] = 0.0f;
		m[5] = 2.0f / (top - bottom);
		m[6] = 0.0f;
		m[7] = 0.0f;
		m[8] = 0.0f;
		m[9] = 0.0f;
		m[10] = -2.0f / (far - near);
		m[11] = 0.0f;
		m[12] = -((right + left) / (right - left));
		m[13] = -((top + bottom) / (top - bottom));
		m[14] = -((far + near) / (far - near));
		m[15] = 1.0f;
	}

	inline void mat4x4_set_frustum(mat4x4 m, float left, float right, float bottom, float top, float near, float far)
	{
		m[0] = (2.0f * near) / (right - left);
		m[1] = 0.0f;
		m[2] = 0.0f;
		m[3] = 0.0f;
		m[4] = 0.0f;
		m[5] = (2.0f * near) / (top - bottom);
		m[6] = 0.0f;
		m[7] = 0.0f;
		m[8] = (right + left) / (right - left);
		m[9] = (top + bottom) / (top - bottom);
		m[10] = -((far + near) / (far - near));
		m[11] = -1.0f;
		m[12] = 0.0f;
		m[13] = 0.0f;
		m[14] = (-2.0f * far * near) / (far - near);
		m[15] = 0.0f;
	}

	inline void mat4x4_set_perspective(mat4x4 m, float fovy, float aspect, float zNear, float zFar)
	{
		float xmin, xmax, ymin, ymax;
		float left, right, bottom, top, near, far;
		ymax = zNear * tan(fovy * 0.00872664626f); //0.00872664626f = PI / 360.0
		ymin = -ymax;
		xmin = ymin * aspect;
		xmax = ymax * aspect;
		//mat4x4_set_frustum(m, xmin, xmax, ymin, ymax, zNear, zFar);
		left = xmin;
		right = xmax;
		bottom = ymin;
		top = ymax;
		near = zNear;
		far = zFar;
		m[0] = (2.0f * near) / (right - left);
		m[1] = 0.0f;
		m[2] = 0.0f;
		m[3] = 0.0f;
		m[4] = 0.0f;
		m[5] = (2.0f * near) / (top - bottom);
		m[6] = 0.0f;
		m[7] = 0.0f;
		m[8] = (right + left) / (right - left);
		m[9] = (top + bottom) / (top - bottom);
		m[10] = -((far + near) / (far - near));
		m[11] = -1.0f;
		m[12] = 0.0f;
		m[13] = 0.0f;
		m[14] = (-2.0f * far * near) / (far - near);
		m[15] = 0.0f;
	}

	inline void mat4x4_set_viewport(
		mat4x4 m, 
		int x, 
		int y, 
		int w, 
		int h,
		float depthMax
		){
		float vx = x;
		float vy = y;
		float widthOverTwo = ((float)(w)) * 0.5f;
		float heightOverTwo = ((float)(h)) * 0.5f;
		float depthMaxOverTwo = depthMax / 2;

		m[0] = widthOverTwo;
		m[1] = 0.0f;
		m[2] = 0.0f;
		m[3] = 0.0f;
		m[4] = 0.0f;
		m[5] =	-heightOverTwo;
		m[6] = 0.0f;
		m[7] = 0.0f;
		m[8] = 0.0f;
		m[9] = 0.0f;
		m[10] = depthMaxOverTwo;
		m[11] = 0.0f;
		m[12] = vx + widthOverTwo;
		m[13] = vy + heightOverTwo;
		m[14] = depthMaxOverTwo;
		m[15] = 1.0f;
	}


	inline void mat4x4_set_look_at(mat4x4 m, vec3f* eye, vec3f* at, vec3f* up)
	{
		vec3f back, right, tup, nup;

		//calculate z
		back.x = eye->x - at->x;
		back.y = eye->y - at->y;
		back.z = eye->z - at->z;
		vec3f_norm(&back, &back);

		//calculate x
		vec3f_norm(&nup, up);
		vec3f_cross(&right, &nup, &back);
		vec3f_norm(&right, &right);
		
		//calculate y
		vec3f_cross(&tup, &back, &right);
		vec3f_norm(&tup, &tup);

		//fill in view matrix 
		m[M4x4_11] = right.x;	
		m[M4x4_21] = tup.x;
		m[M4x4_31] = back.x;
		m[M4x4_41] = 0;

		m[M4x4_12] = right.y;	
		m[M4x4_22] = tup.y;
		m[M4x4_32] = back.y;
		m[M4x4_42] = 0;

		m[M4x4_13] = right.z;	
		m[M4x4_23] = tup.z;
		m[M4x4_33] = back.z;
		m[M4x4_43] = 0;

		m[M4x4_14] = -vec3f_dot(eye, &right);
		m[M4x4_24] = -vec3f_dot(eye, &tup);
		m[M4x4_34] = -vec3f_dot(eye, &back);
		m[M4x4_44] = 1.0f;


	}

	inline void mat4x4_get_axis_u(vec3f* result, mat4x4 m)
	{
		result->x = m[0];
		result->y = m[4];
		result->z = m[8];
	}

	inline void mat4x4_get_axis_v(vec3f* result, mat4x4 m)
	{
		result->x = m[1];
		result->y = m[5];
		result->z = m[9];
	}

	inline void mat4x4_get_axis_n(vec3f* result, mat4x4 m)
	{
		result->x = m[2];
		result->y = m[6];
		result->z = m[10];
	}

	inline void mat4x4_transform(vec4f* v, mat4x4 m)
	{
		float tx = m[0] * v->x + m[4] * v->y + m[8] * v->z + m[12] * v->w;
		float ty = m[1] * v->x + m[5] * v->y + m[9] * v->z + m[13] * v->w;
		float tz = m[2] * v->x + m[6] * v->y + m[10] * v->z + m[14] * v->w;
		float tw = m[3] * v->x + m[7] * v->y + m[11] * v->z + m[15] * v->w;
		v->x = tx; v->y = ty; v->z = tz; v->w = tw;
	}

	//result = a*b
	inline void mat4x4_multiply(mat4x4 result, mat4x4 a, mat4x4 b)
	{
		mat4x4 tmp;
		//tmp = a * b;
		tmp[0] = a[0] * b[0] + a[4] * b[1] + a[8] * b[2] + a[12] * b[3];
		tmp[1] = a[1] * b[0] + a[5] * b[1] + a[9] * b[2] + a[13] * b[3];
		tmp[2] = a[2] * b[0] + a[6] * b[1] + a[10] * b[2] + a[14] * b[3];
		tmp[3] = a[3] * b[0] + a[7] * b[1] + a[11] * b[2] + a[15] * b[3];
		tmp[4] = a[0] * b[4] + a[4] * b[5] + a[8] * b[6] + a[12] * b[7];
		tmp[5] = a[1] * b[4] + a[5] * b[5] + a[9] * b[6] + a[13] * b[7];
		tmp[6] = a[2] * b[4] + a[6] * b[5] + a[10] * b[6] + a[14] * b[7];
		tmp[7] = a[3] * b[4] + a[7] * b[5] + a[11] * b[6] + a[15] * b[7];
		tmp[8] = a[0] * b[8] + a[4] * b[9] + a[8] * b[10] + a[12] * b[11];
		tmp[9] = a[1] * b[8] + a[5] * b[9] + a[9] * b[10] + a[13] * b[11];
		tmp[10] = a[2] * b[8] + a[6] * b[9] + a[10] * b[10] + a[14] * b[11];
		tmp[11] = a[3] * b[8] + a[7] * b[9] + a[11] * b[10] + a[15] * b[11];
		tmp[12] = a[0] * b[12] + a[4] * b[13] + a[8] * b[14] + a[12] * b[15];
		tmp[13] = a[1] * b[12] + a[5] * b[13] + a[9] * b[14] + a[13] * b[15];
		tmp[14] = a[2] * b[12] + a[6] * b[13] + a[10] * b[14] + a[14] * b[15];
		tmp[15] = a[3] * b[12] + a[7] * b[13] + a[11] * b[14] + a[15] * b[15];
		//result = tmp
		result[0] = tmp[0];
		result[1] = tmp[1];
		result[2] = tmp[2];
		result[3] = tmp[3];
		result[4] = tmp[4];
		result[5] = tmp[5];
		result[6] = tmp[6];
		result[7] = tmp[7];
		result[8] = tmp[8];
		result[9] = tmp[9];
		result[10] = tmp[10];
		result[11] = tmp[11];
		result[12] = tmp[12];
		result[13] = tmp[13];
		result[14] = tmp[14];
		result[15] = tmp[15];
	}

	inline void mat4x4_extract_scale(vec3f* result, mat4x4 mat){
		result->x = mat[M4x4_11];
		result->y = mat[M4x4_22];
		result->z = mat[M4x4_33];
	}

	//based on 
	inline void mat4x4_extract_clipping_planes(plane p[6], mat4x4 m) 
	{
		//left plane
		p[0].A = m[M4x4_41] + m[M4x4_11];
		p[0].B = m[M4x4_42] + m[M4x4_12];
		p[0].C = m[M4x4_43] + m[M4x4_13];
		p[0].D = m[M4x4_44] + m[M4x4_14];

		//right plane
		p[1].A = m[M4x4_41] - m[M4x4_11];
		p[1].B = m[M4x4_42] - m[M4x4_12];
		p[1].C = m[M4x4_43] - m[M4x4_13];
		p[1].D = m[M4x4_44] - m[M4x4_14];

		//top plane
		p[2].A = m[M4x4_41] - m[M4x4_21];
		p[2].B = m[M4x4_42] - m[M4x4_22];
		p[2].C = m[M4x4_43] - m[M4x4_23];
		p[2].D = m[M4x4_44] - m[M4x4_24];

		//bottom plane
		p[3].A = m[M4x4_41] + m[M4x4_21];
		p[3].B = m[M4x4_42] + m[M4x4_22];
		p[3].C = m[M4x4_43] + m[M4x4_23];
		p[3].D = m[M4x4_44] + m[M4x4_24];

		//near plane
		p[4].A = m[M4x4_41] + m[M4x4_31];
		p[4].B = m[M4x4_42] + m[M4x4_32];
		p[4].C = m[M4x4_43] + m[M4x4_33];
		p[4].D = m[M4x4_44] + m[M4x4_34];

		//far plane
		p[5].A = m[M4x4_41] - m[M4x4_31];
		p[5].B = m[M4x4_42] - m[M4x4_32];
		p[5].C = m[M4x4_43] - m[M4x4_33];
		p[5].D = m[M4x4_44] - m[M4x4_34];

		//normalize planes
		for (int i = 0; i < 6; i++) {
			plane_normalize(&p[i], &p[i]);
		}
		
	}
}



#endif
