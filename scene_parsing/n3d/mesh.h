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
		vec3f normal;
		texcoord2f uv;
	};

	class Mesh
	{
	public:
		Mesh();
		Mesh(const Mesh& other);
		Mesh* Clone();
		void LoadObjectFile(const char* filepath, n3d::vec3f& preScale);
		inline int GetNumOfTriangles() {
			return numTriangles;
		}
	private:
		void ParsePointLine(const std::string& line, vec3f& vertex);
		void ParseNormalLine(const std::string& line, vec3f& normal);
		void ParseTextureCoordinateLine(const std::string& line, 
			texcoord2f& texcoord);
		void ParseTriangleLine(std::string& line, vec3i& p, vec3i& q, vec3i& r);

		
	public:
		std::vector<MeshVertex> vertices;
		int numTriangles;
		n3d::vec3f center;
		float radius;

	};
}

#endif