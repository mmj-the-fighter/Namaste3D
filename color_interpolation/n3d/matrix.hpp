#ifndef _N3D_MATRIX_HPP_
#define _N3D_MATRIX_HPP_

#include "vector.hpp"

namespace n3d{
#define TORADIANS(x)	((x)*0.0174532925f)
#define TODEGREES(x)	((x)*57.295779513f)

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
		result[0] = m[0];
		result[1] = m[1];
		result[2] = m[2];
		result[3] = m[4];
		result[4] = m[5];
		result[5] = m[6];
		result[6] = m[8];
		result[7] = m[9];
		result[8] = m[10];
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
		float det = mat3x3_determinant(m);
		if (det == 0.0f) {
			return 1;
		}
		float oneOverDet = 1.0f / det;
		result[0] = oneOverDet * (m[4] * m[8] - m[5] * m[7]);
		result[1] = oneOverDet * (m[2] * m[7] - m[1] * m[8]);
		result[2] = oneOverDet * (m[1] * m[5] - m[2] * m[4]);
		result[3] = oneOverDet * (m[5] * m[6] - m[3] * m[8]);
		result[4] = oneOverDet * (m[0] * m[8] - m[2] * m[6]);
		result[5] = oneOverDet * (m[2] * m[3] - m[0] * m[5]);
		result[6] = oneOverDet * (m[3] * m[7] - m[4] * m[6]);
		result[7] = oneOverDet * (m[1] * m[6] - m[0] * m[7]);
		result[8] = oneOverDet * (m[0] * m[4] - m[1] * m[3]);
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
		vec3f u, v, n;
		//n = eye - at
		n.x = eye->x - at->x;
		n.y = eye->y - at->y;
		n.z = eye->z - at->z;

		//n = |n|
		vec3f_norm(&n, &n);
		//up = |up|
		vec3f_norm(up, up);
		//u = up X n
		vec3f_cross(&u, up, &n);
		//u = |u|
		vec3f_norm(&u, &u);
		//v = n X u
		vec3f_cross(&v, &n, &u);
		//fill in view matrix 
		m[0] = u.x;	m[4] = u.y;	m[8] = u.z;	m[12] = -vec3f_dot(eye, &u);
		m[1] = v.x;	m[5] = v.y;	m[9] = v.z;	m[13] = -vec3f_dot(eye, &v);
		m[2] = n.x;	m[6] = n.y;	m[10] = n.z; m[14] = -vec3f_dot(eye, &n);
		m[3] = 0;	m[7] = 0;	m[11] = 0;	m[15] = 1.0f;
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
}



#endif
