#ifndef _N3D_VECTOR_HPP_
#define _N3D_VECTOR_HPP_


#include <cmath>
namespace n3d
{
	typedef struct _vec2f
	{
		union{
			float v[2];
			struct{
				float x;
				float y;
			};
		};
	}vec2f;

	typedef struct _vec2i
	{
		union{
			int v[2];
			struct{
				int x;
				int y;
			};
		};
	}vec2i;

	typedef struct _vec3f
	{
		union{
			float v[3];
			struct {
				float x;
				float y;
				float z;
			};
		};
	}vec3f;

	typedef struct _vec3i
	{
		union {
			int v[3];
			struct {
				int x;
				int y;
				int z;
			};
		};
	}vec3i;

	typedef struct _color3i
	{
		union {
			int bgr[3];
			struct{
				int b;
				int g;
				int r;
			};
		};
	}color3i;

	typedef struct _color3f
	{
		union {
			float bgr[3];
			struct {
				float b;
				float g;
				float r;
			};
		};
	}color3f;

	typedef struct _vec4f
	{
		union {
			float v[4];
			struct {
				float x;
				float y;
				float z;
				float w;
			};
		};
	}vec4f;

	typedef struct _vec4i
	{
		union{
			int v[4];
			struct{
				int x;
				int y;
				int z;
				int w;
			};
		};
	}vec4i;

	typedef struct _texcoord2f
	{
		union{
			float st[2];
			struct {
				float s;
				float t;
			};
		};
	}texcoord2f;

	inline void vec3f_set(vec3f* result, float x, float y, float z)
	{
		result->x = x;
		result->y = y;
		result->z = z;
	}

	inline void vec3f_scale(vec3f* result, vec3f* v, float s)
	{
		result->x = v->x * s;
		result->y = v->y * s;
		result->z = v->z * s;
	}

	inline void vec3f_scale2(vec3f* result, vec3f v, float s)
	{
		result->x = v.x * s;
		result->y = v.y * s;
		result->z = v.z * s;
	}
	
	/* result = b - a  */
	inline void vec3f_sub(vec3f* result, vec3f* a, vec3f* b)
	{
		result->x = b->x - a->x;
		result->y = b->y - a->y;
		result->z = b->z - a->z;
	}

	/* result = b - a  */
	inline void vec3f_sub2(vec3f* result, vec3f a, vec3f b)
	{
		result->x = b.x - a.x;
		result->y = b.y - a.y;
		result->z = b.z - a.z;
	}

	inline void vec3f_add(vec3f* result, vec3f* a, vec3f* b)
	{
		result->x = a->x + b->x;
		result->y = a->y + b->y;
		result->z = a->z + b->z;
	}

	inline void vec3f_add2(vec3f* result, vec3f a, vec3f b)
	{
		result->x = a.x + b.x;
		result->y = a.y + b.y;
		result->z = a.z + b.z;
	}

	inline void vec3f_scale_and_add(vec3f* result, float scale, vec3f* a, vec3f* b) {
		result->x = a->x * scale + b->x;
		result->y = a->y * scale + b->y;
		result->z = a->z * scale + b->z;
	}

	inline void vec3f_scale_and_add2(vec3f* result, float scale, vec3f a, vec3f b) {
		result->x = a.x * scale + b.x;
		result->y = a.y * scale + b.y;
		result->z = a.z * scale + b.z;
	}

	inline float vec3f_dot(vec3f* a, vec3f* b)
	{
		return (a->x * b->x + a->y * b->y + a->z * b->z);
	}

	inline float vec3f_dot2(vec3f a, vec3f b)
	{
		return (a.x * b.x + a.y * b.y + a.z * b.z);
	}

	inline void vec3f_cross(vec3f* result, vec3f* a, vec3f* b)
	{
		result->x = a->y * b->z - a->z * b->y;
		result->y = a->z * b->x - a->x * b->z;
		result->z = a->x * b->y - a->y * b->x;
	}

	inline void vec3f_cross2(vec3f* result, vec3f a, vec3f b)
	{
		result->x = a.y * b.z - a.z * b.y;
		result->y = a.z * b.x - a.x * b.z;
		result->z = a.x * b.y - a.y * b.x;
	}

	inline float vec3f_mag(vec3f* v)
	{
		return sqrtf(v->x * v->x + v->y * v->y + v->z * v->z);
	}

	inline float vec3f_mag2(vec3f v)
	{
		return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
	}

	inline float vec3f_mag_squared(vec3f* v)
	{
		return (v->x * v->x + v->y * v->y + v->z * v->z);
	}

	inline float vec3f_mag_squared2(vec3f v)
	{
		return (v.x * v.x + v.y * v.y + v.z * v.z);
	}

	inline void vec3f_norm(vec3f* result, vec3f* v)
	{
		double t = (double)(v->x * v->x + v->y * v->y + v->z * v->z);
		t = 1.0 / sqrt(t);
		result->x = v->x * t;
		result->y = v->y * t;
		result->z = v->z * t;
	}

	inline void vec3f_norm2(vec3f* result, vec3f v)
	{
		double t = (double)(v.x * v.x + v.y * v.y + v.z * v.z);
		t = 1.0 / sqrt(t);
		result->x = v.x * t;
		result->y = v.y * t;
		result->z = v.z * t;
	}

	inline void vec4f_make_point_from_vec3f(vec4f* result, vec3f* v){
		result->x = v->x;
		result->y = v->y;
		result->z = v->z;
		result->w = 1.0f;
	}

	inline void vec4f_make_point_from_vec3f2(vec4f* result, vec3f v){
		result->x = v.x;
		result->y = v.y;
		result->z = v.z;
		result->w = 1.0f;
	}

	inline void vec4f_lerp(vec4f *result, vec4f*a, vec4f* b, float t){
		result->x = a->x + (b->x - a->x) * t;
		result->y = a->y + (b->y - a->y) * t;
		result->z = a->z + (b->z - a->z) * t;
		result->w = a->w + (b->w - a->w) * t;
	}

	inline void vec4f_lerp2(vec4f *result, vec4f a, vec4f b, float t){
		result->x = a.x + (b.x - a.x) * t;
		result->y = a.y + (b.y - a.y) * t;
		result->z = a.z + (b.z - a.z) * t;
		result->w = a.w + (b.w - a.w) * t;
	}

	inline void texcoord2f_lerp(texcoord2f* result, texcoord2f* a, texcoord2f* b, float t) {
		result->s = a->s + (b->s - a->s) * t;
		result->t = a->t + (b->t - a->t) * t;
	}

	inline void texcoord2f_lerp2(texcoord2f* result, texcoord2f a, texcoord2f b, float t) {
		result->s = a.s + (b.s - a.s) * t;
		result->t = a.t + (b.t - a.t) * t;
	}

	inline void color3f_lerp(color3f* result, 
		color3f* a, 
		color3f* b, 
		float t) 
	{
		result->b = a->b + (b->b - a->b) * t;
		result->g = a->g + (b->g - a->g) * t;
		result->r = a->r + (b->r - a->r) * t;
	}

	inline void color3f_lerp2(color3f* result,
		color3f a,
		color3f b,
		float t)
	{
		result->b = a.b + (b.b - a.b) * t;
		result->g = a.g + (b.g - a.g) * t;
		result->r = a.r + (b.r - a.r) * t;
	}

	inline float compute_two_times_triangle_area_vec4f(vec4f* p0, vec4f *p1, vec4f* p2)
	{
		float twoTimesArea = ((p0->x*p1->y - p1->x*p0->y) +
			(p1->x*p2->y - p2->x*p1->y) +
			(p2->x*p0->y - p0->x*p2->y));
		return twoTimesArea;
	}
}

#endif