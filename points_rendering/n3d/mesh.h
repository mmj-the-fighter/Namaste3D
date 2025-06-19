#ifndef _N3D_MESH_H_
#define _N3D_MESH_H_

#include "vector.hpp"
#include "../../spinach/core/spn_image.h"
#include <vector>
#include <string>

namespace n3d{
	struct MeshVertex
	{
		vec3f position;
	};

	class Mesh
	{
	public:
		Mesh();
		void LoadObjectFile(const char* filepath);
		void ParsePointLine(const std::string& line, vec3f& vertex);
	public:
		std::vector<MeshVertex> vertices;
	};
}

#endif