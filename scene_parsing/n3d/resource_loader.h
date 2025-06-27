#ifndef _N3D_RESOURCE_LOADER_H_
#define _N3D_RESOURCE_LOADER_H_
#include <unordered_map>
#include <string>

#include "../../spinach/core/spn_image.h"

#include "mesh.h"

namespace n3d {

	class ResourceLoader
	{
	public:
		~ResourceLoader();
		spn::Image* GetImage(int id);
		void PutImage(int id, std::string fileName);
		n3d::Mesh* GetMesh(int id);
		void PutMesh(int id, std::string fileName, n3d::vec3f& preScale);
		void FreeResources();


	private:
		std::unordered_map<int, n3d::Mesh*> meshStorage;
		std::unordered_map<int, spn::Image*> imageStorage;

	};

	

}
#endif