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

	typedef struct _float2
	{
		union{
			float v[2];
			struct{
				float x;
				float y;
			};
		};
	}float2;


	typedef struct _float3
	{
		union{
			float v[3];
			struct{
				float x;
				float y;
				float z;
			};
		};
	}float3;

	typedef struct _float4
	{
		union{
			float v[4];
			struct{
				float x;
				float y;
				float z;
				float w;
			};
		};
	}float4;


	typedef struct _float5
	{
		union{
			float v[5];
			struct{
				float a;
				float b;
				float c;
				float d;
				float e;
			};
		};
	}float5;


	typedef struct _float6
	{
		union{
			float v[6];
			struct{
				float a;
				float b;
				float c;
				float d;
				float e;
				float f;
			};
		};
	}float6;

	typedef struct _texcoord2f
	{
		union{
			float uv[2];
			struct {
				float u;
				float v;
			};
		};
	}texcoord2f;
	

	inline void vec3i_reverse_order(vec3i* result, vec3i* source)
	{
		float t = source->x;
		result->x = source->z;
		result->y = source->y;
		result->z = t;
	}

	inline void vec3f_reverse_order(vec3f* result, vec3f* source)
	{
		float t = source->x;
		result->x = source->z;
		result->y = source->y;
		result->z = t;
	}

	inline void color3f_reverse_order(color3f* result, color3f* source)
	{
		float t = source->b;
		result->b = source->r;
		result->g = source->g;
		result->r = t;
	}

	inline void color3i_reverse_order(color3i* result, color3i* source)
	{
		float t = source->b;
		result->b = source->r;
		result->g = source->g;
		result->r = t;
	}

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

	inline void vec3f_scale_Hadamard(vec3f* result, vec3f* v, vec3f* s)
	{
		result->x = v->x * s->x;
		result->y = v->y * s->y;
		result->z = v->z * s->z;
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

	inline float vec3f_distance_squared(vec3f *a, vec3f* b){
		float dx = b->x - a->x;
		float dy = b->y - a->y;
		float dz = b->z - a->z;
		return (dx * dx + dy * dy + dz * dz);
	}

	inline float vec3f_distance(vec3f *a, vec3f* b){
		float dx = b->x - a->x;
		float dy = b->y - a->y;
		float dz = b->z - a->z;
		return sqrtf(dx * dx + dy * dy + dz * dz);
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

	inline void vec3f_lerp(vec3f *result, vec3f*a, vec3f* b, float t){
		result->x = a->x + (b->x - a->x) * t;
		result->y = a->y + (b->y - a->y) * t;
		result->z = a->z + (b->z - a->z) * t;
	}


	inline void vec4f_lerp2(vec4f *result, vec4f a, vec4f b, float t){
		result->x = a.x + (b.x - a.x) * t;
		result->y = a.y + (b.y - a.y) * t;
		result->z = a.z + (b.z - a.z) * t;
		result->w = a.w + (b.w - a.w) * t;
	}

	inline void texcoord2f_lerp(texcoord2f* result, texcoord2f* a, texcoord2f* b, float t) {
		result->u = a->u + (b->u - a->u) * t;
		result->v = a->v + (b->v - a->v) * t;
	}

	inline void texcoord2f_lerp2(texcoord2f* result, texcoord2f a, texcoord2f b, float t) {
		result->u = a.u + (b.u - a.u) * t;
		result->v = a.v + (b.v - a.v) * t;
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

	inline void vec3f_extract_from_vec4f(vec3f* result, vec4f* v) {
		result->x = v->x;
		result->y = v->y;
		result->z = v->z;
	}
}

#endif