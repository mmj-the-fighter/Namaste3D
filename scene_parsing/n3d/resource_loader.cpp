#include <iostream>
#include "mesh.h"
#include "resource_loader.h"


namespace n3d {

	ResourceLoader::~ResourceLoader(){
		FreeResources();
	}

	spn::Image* ResourceLoader::GetImage(int id)
	{
		auto i = imageStorage.find(id);
		if (i == imageStorage.end()) {
			//std::cout << "Could n't find image with id: " << id << "\n";
			return nullptr;
		}
		return i->second;
	}

	void ResourceLoader::PutImage(int id, std::string fileName)
	{
		spn::Image* image = new spn::Image();
		image->CreateFromPng(fileName.c_str());
		imageStorage[id] = image;
	}

	n3d::Mesh* ResourceLoader::GetMesh(int id)
	{
		auto i = meshStorage.find(id);
		if (i == meshStorage.end()) {
			//std::cout << "Could n't find mesh with id: " << id << "\n";
			return nullptr;
		}
		return i->second;

	}

	void ResourceLoader::PutMesh(int id, std::string fileName, n3d::vec3f& preScale)
	{
		Mesh* mesh = new Mesh;
		mesh->LoadObjectFile(fileName.c_str(), preScale);
		meshStorage[id] = mesh;
	}

	void ResourceLoader::FreeResources() {
		for (auto& pair : imageStorage){
			if (pair.second != nullptr) {
				delete pair.second;
				pair.second = nullptr;
			}
		}

		for (auto& pair : meshStorage){
			if (pair.second != nullptr) {
				delete pair.second;
				pair.second = nullptr;
			}
		}

	}

}
