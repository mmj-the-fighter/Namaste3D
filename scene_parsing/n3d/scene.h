#ifndef _N3D_SCN_H_
#define _N3D_SCN_H_

#include <vector>
#include <string>
#include "vector.hpp"
#include "matrix.hpp"
#include "mesh.h"
#include "n3d.h"
#include "resource_loader.h"
#include "../../spinach/core/spn_image.h"



namespace n3d{

	struct ProjectionPart
	{
		float fovy;
		float near;
		float far;

		float left;
		float right;
		float top;
		float bottom;
		float orthoNear;
		float orthoFar;
		int id;
	};

	struct ViewPart
	{
		vec3f eye;
		vec3f at;
		vec3f up;
		int id;
	};

	struct MeshPart
	{
		n3d::Mesh* mesh;
		std::string filename;
		n3d::vec3f preScale;
		int id;
	};

	struct TexturePart
	{
		spn::Image* image;
		std::string filename;
		int id;
	};

	struct MaterialPart
	{
		Material material;
		int id;
	};

	struct PointLightPart
	{
		PointLight pointLight;
		int id;
	};

	struct GeometricObjectPart
	{
		int id;
		int parentId;
		int meshId;
		int textureId;
		int materialId;
		vec3f localScale;
		vec3f localRotation;
		vec3f localTranslation;
		n3d::mat4x4 modelMat;
		spn::Image *texture;
		n3d::Mesh *mesh;
		n3d::Material* material;
	};

	struct InitializerPart
	{
		int projectionId;
		int viewId;
		int pointLightId;
	};

	struct IndexPair{
		int firstIndex;
		int secondIndex;
	};

	/*class GeometricObjectNode{
		int index;
	public:
		int GetIndex();
		int GetRootIndex();
		void AddChild(int index);
		void Traverse();
	};*/
	
	class Scene
	{
	public:
		Scene(ResourceLoader *pResourceLoader);
		~Scene();
		bool LoadSceneFile(const char* filepath);
		void UnLoadScene();
		void GetInitializedParts(ProjectionPart& projectionPart,
			ViewPart& viewPart,
			PointLightPart &pointLightPart
			);
		void GeometryStart() {
			geometryIndex = 0;
		}
		void GetGeometryAndIterate(GeometricObjectPart& geom){
			if (geometryIndex < geometricObjectParts.size()){
				geom = geometricObjectParts[geometryIndex];
				++geometryIndex;
			}
		}
		bool IsGeometryEnded() {
			if (geometryIndex < geometricObjectParts.size()){
				return false;
			}
			else{
				return true;
			}
		}
		//spn::Image* GetTexturePointer(GeometricObjectPart &geom){
		//	for (const auto& t : textureParts){
		//		if (t.id == geom.textureId){
		//			return t.image;
		//		}
		//	}
		//	return nullptr;
		//}

		//Mesh* GetMeshPointer(GeometricObjectPart &geom){
		//	for (const auto& t : meshParts){
		//		if (t.id == geom.meshId){
		//			return t.mesh;
		//		}
		//	}
		//	return nullptr;
		//}

		//void GetMaterial(GeometricObjectPart &geom, Material& m){
		//	for (const auto& t : materialParts){
		//		if (t.id == geom.materialId){
		//			m.ambient = t.material.ambient;
		//			m.diffuse = t.material.diffuse;
		//			return;
		//		}
		//	}
		//	m.ambient.x = m.ambient.y = m.ambient.z = 0;
		//	m.diffuse.x = m.diffuse.y = m.diffuse.z = 0;
		//}

	private:
		int geometryIndex = 0;
		void ShowError(std::string& expected, int lineNo);
		bool DoesLineStartWithExpectedKeyword (
			std::string& line, 
			int lineNo,
			int &marker,
			std::vector<std::string>& keywords,
			int expectedKeywordIndex);
		int ReadStringFromLine(char *buf, int bufLen, std::string& line, int startIndex);
		int KeywordIndex(char* str, std::vector<std::string>& keywords);
		int KeyWordIndex(const std::string& s, std::vector<std::string>& keywords);
		void ReadInt(int &i1, std::string& line, int& marker);
		void ReadVec3f(n3d::vec3f& v, std::string& line, int& marker);
		void ReadColor3f(n3d::color3f& v, std::string& line, int& marker);
		void ReadFloat(float &f1, std::string& line, int& marker);
		void ReadFloat2(n3d::float2 &f2, std::string& line, int& marker);
		void ReadFloat3(n3d::float3 &f3, std::string& line, int& marker);
		void ReadFloat4(n3d::float4 &f4, std::string& line, int& marker);
		void ReadFloat5(n3d::float5 &f5, std::string& line, int& marker);
		void ReadFloat6(n3d::float6 &f6, std::string& line, int& marker);
		bool ParseProjectionPart();
		bool ParseViewPart();
		bool ParseMeshPart();
		bool ParseTexturePart();
		bool ParseMaterialPart();
		bool ParsePointLightPart();
		bool ParseGeometricObjectPart();
		bool ParseInitializerPart();
		//void BuildSceneGraph();
	private:
		int numOfComments;
		std::vector<int> commentsSoFarAtEachLine;
		std::vector<std::string> partsBeginKeywords;
		std::vector<std::string> partsEndKeywords;

		std::vector<std::string> projectionKeywords;
		std::vector<std::string> viewKeywords;
		std::vector<std::string> meshKeywords;
		std::vector<std::string> textureKeywords;
		std::vector<std::string> materialKeywords;
		std::vector<std::string> pointLightKeywords;
		std::vector<std::string> geometricObjectKeywords;
		std::vector<std::string> initializerKeywords;

		std::vector<ProjectionPart> projectionParts;
		std::vector<ViewPart> viewParts;
		std::vector<MeshPart> meshParts;
		std::vector<TexturePart> textureParts;
		std::vector<MaterialPart> materialParts;
		std::vector<PointLightPart> pointLightParts;
		std::vector<GeometricObjectPart> geometricObjectParts;
		std::vector<InitializerPart> initializerParts;

		std::vector<IndexPair> projectionLines;
		std::vector<IndexPair> viewLines;
		std::vector<IndexPair> meshLines;
		std::vector<IndexPair> textureLines;
		std::vector<IndexPair> materialsLines;
		std::vector<IndexPair> pointLightLines;
		std::vector<IndexPair> geometricObjectLines;
		std::vector<IndexPair> initializerLines;
		std::vector<std::string> fileLines;
		ResourceLoader* resourceLoader;
	};
}

#endif