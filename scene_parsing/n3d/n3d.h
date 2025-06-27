#ifndef _N3D_H_
#define _N3D_H_
#include "vector.hpp"
#include "matrix.hpp"

namespace n3d {

	struct SwrVertex
	{
		vec4f position;
		color3f color;
		texcoord2f texcoord;
	};

	struct SwrTriangle
	{
		union {
			SwrVertex v[3];
			struct {
				SwrVertex a;
				SwrVertex b;
				SwrVertex c;
			};
		};
	};

	struct PointLight
	{
		vec3f viewSpaceLocation;
		vec4f worldSpaceLocation;
		color3f ambience;
		color3f diffuse;
	};

	struct Material
	{
		vec3f ambient;
		vec3f diffuse;
	};

}
#endif