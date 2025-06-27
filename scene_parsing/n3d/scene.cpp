#include <fstream>
#include <string>
#include <iostream>


#include "scene.h"

namespace n3d {

	Scene::~Scene() {
		UnLoadScene();
	}

	Scene::Scene(ResourceLoader* pResourceLoader) {
		resourceLoader = pResourceLoader;
		numOfComments = 0;
		partsBeginKeywords.push_back("projection_begin");
		partsBeginKeywords.push_back("view_begin");
		partsBeginKeywords.push_back("mesh_begin");
		partsBeginKeywords.push_back("texture_begin");
		partsBeginKeywords.push_back("material_begin");
		partsBeginKeywords.push_back("point_light_begin");
		partsBeginKeywords.push_back("geom_object_begin");
		partsBeginKeywords.push_back("initializer_begin");

		partsEndKeywords.push_back("projection_end");
		partsEndKeywords.push_back("view_end");
		partsEndKeywords.push_back("mesh_end");
		partsEndKeywords.push_back("texture_end");
		partsEndKeywords.push_back("material_end");
		partsEndKeywords.push_back("point_light_end");
		partsEndKeywords.push_back("geom_object_end");
		partsEndKeywords.push_back("initializer_end");

		projectionKeywords.push_back("id");
		projectionKeywords.push_back("perspective");
		projectionKeywords.push_back("ortho");

		viewKeywords.push_back("id");
		viewKeywords.push_back("eye");
		viewKeywords.push_back("at");
		viewKeywords.push_back("up");

		meshKeywords.push_back("id");
		meshKeywords.push_back("filename");
		meshKeywords.push_back("scale");

		textureKeywords.push_back("id");
		textureKeywords.push_back("filename");

		materialKeywords.push_back("id");
		materialKeywords.push_back("ambient");
		materialKeywords.push_back("diffuse");

		pointLightKeywords.push_back("id");
		pointLightKeywords.push_back("ambient");
		pointLightKeywords.push_back("diffuse");
		pointLightKeywords.push_back("trans");

		geometricObjectKeywords.push_back("id");
		geometricObjectKeywords.push_back("mesh_id");
		geometricObjectKeywords.push_back("texture_id");
		geometricObjectKeywords.push_back("material_id");
		geometricObjectKeywords.push_back("scale");
		geometricObjectKeywords.push_back("rot");
		geometricObjectKeywords.push_back("trans");
		geometricObjectKeywords.push_back("parent_id");

		initializerKeywords.push_back("projection_id");
		initializerKeywords.push_back("view_id");
		initializerKeywords.push_back("point_light_id");
	}

	void Scene::UnLoadScene()
	{
		projectionParts.clear();
		viewParts.clear();

		/*for (auto& m : meshParts){
			delete m.mesh;
		}*/
		meshParts.clear();

		/*for (auto& t : textureParts){
			delete t.image;
		}*/
		textureParts.clear();
		materialParts.clear();
		pointLightParts.clear();
		geometricObjectParts.clear();
		initializerParts.clear();
		resourceLoader->FreeResources();
	}

	bool Scene::LoadSceneFile(const char* filepath) {
		std::ifstream file(filepath);
		if (!file.is_open()) {
			std::cerr << "Failed to open scene file: " <<filepath<< std::endl;
			return false;
		}
		
		fileLines.clear();
		commentsSoFarAtEachLine.clear();
		std::string line;
		numOfComments = 0;
		while (std::getline(file, line)) {
			if (line[0] != '#') {
				fileLines.push_back(line);
				commentsSoFarAtEachLine.push_back(numOfComments);
			}
			else{
				++numOfComments;
			}
		}
		UnLoadScene();

		projectionLines.clear();
		viewLines.clear();
		meshLines.clear();
		textureLines.clear();
		materialsLines.clear();
		pointLightLines.clear();
		geometricObjectLines.clear();
		initializerLines.clear();


		int n = fileLines.size();
		int startIndex = -999;
		int lineStart = -1;
		int commentsAtLineStart = 0;
		for (int i = 0; i < n; ++i)
		{
			const std::string& fl = fileLines[i];
			const int commentsSofarUptoThisLine = commentsSoFarAtEachLine[i];
			int beginIndex = KeyWordIndex(fl, partsBeginKeywords);
			if (beginIndex != -1){
				startIndex = beginIndex;
				lineStart = i;
				commentsAtLineStart = commentsSofarUptoThisLine;
				continue;
			}
			int endIndex = KeyWordIndex(fl, partsEndKeywords);
			if (endIndex != -1){
				if (startIndex == endIndex){
					std::cout << "Parse " << partsBeginKeywords[startIndex] << " at lines " << commentsAtLineStart + lineStart + 1 + 1 << " " << commentsSofarUptoThisLine + i + 1 - 1 << "\n";
					IndexPair p;
					p.firstIndex = lineStart + 1;
					p.secondIndex = i - 1;
					switch (startIndex) {
					case 0:
						projectionLines.push_back(p);
						break;
					case 1:
						viewLines.push_back(p);
						break;
					case 2:
						meshLines.push_back(p);
						break;
					case 3:
						textureLines.push_back(p);
						break;
					case 4:
						materialsLines.push_back(p);
						break;
					case 5:
						pointLightLines.push_back(p);
						break;
					case 6:
						geometricObjectLines.push_back(p);
						break;
					case 7:
						initializerLines.push_back(p);
						break;
					}
				}
				else{
					std::cout << "parsing failed " << "expected " << partsEndKeywords[startIndex] << " at line: " << commentsSofarUptoThisLine + i + 1 << "\n";
					return false;
				}
				continue;
			}
		}

		int linesSize;
		bool parsingFailed = false;
		linesSize = projectionLines.size();
		if (linesSize == 0) {
			std::cout << "parsing failed" << " no projection found \n";
			parsingFailed = true;
		}
		linesSize = viewLines.size();
		if (linesSize == 0) {
			std::cout << "parsing failed" << " no view found \n";
			parsingFailed = true;
		}

		linesSize = meshLines.size();
		if (linesSize == 0) {
			std::cout << "parsing failed" << " no mesh found \n";
			parsingFailed = true;
		}

		linesSize = textureLines.size();
		if (linesSize == 0) {
			std::cout << "parsing failed" << " no texture found \n";
			parsingFailed = true;
		}

		linesSize = materialsLines.size();
		if (linesSize == 0) {
			std::cout << "parsing failed" << " no material found \n";
			parsingFailed = true;
		}

		linesSize = pointLightLines.size();
		if (linesSize == 0) {
			std::cout << "parsing failed" << " no point light found \n";
			parsingFailed = true;
		}

		linesSize = geometricObjectLines.size();
		if (linesSize == 0) {
			std::cout << "parsing failed" << " no geometric object found \n";
			parsingFailed = true;
		}

		linesSize = initializerLines.size();
		if (linesSize == 0) {
			std::cout << "parsing failed" << " no initializer found \n";
			parsingFailed = true;
		}
		else if (linesSize > 1){
			std::cout << "parsing failed" << " more than one initializer found \n";
			parsingFailed = true;
		}
		if (parsingFailed) {
			return false;
		}

	
		bool r;
		r = ParseProjectionPart();
		if (!r){
			return false;
		}
		r = ParseViewPart();
		if (!r){
			return false;
		}
		r = ParseMeshPart();
		if (!r){
			return false;
		}

		r = ParseTexturePart();
		if (!r){
			return false;
		}

		r = ParseMaterialPart();
		if (!r){
			return false;
		}

		r = ParsePointLightPart();
		if (!r){
			return false;
		}

		r = ParseGeometricObjectPart();
		if (!r){
			return false;
		}

		r = ParseInitializerPart();
		if (!r){
			return false;
		}

		resourceLoader->FreeResources();
		int nObjects = geometricObjectParts.size();
		for (int i = 0; i < nObjects; i++) {
			GeometricObjectPart& geom = geometricObjectParts[i];
		
			for (auto& t : textureParts){
				if (t.id == geom.textureId){
					spn::Image* image = resourceLoader->GetImage(geom.textureId);
					if (image == nullptr) {
						resourceLoader->PutImage(geom.textureId, t.filename);
					}
					t.image = resourceLoader->GetImage(geom.textureId);
					geom.texture = t.image;
				}
			}

			for (auto& t : meshParts){
				if (t.id == geom.meshId){
					n3d::Mesh* mesh = resourceLoader->GetMesh(geom.meshId);
					if (mesh == nullptr) {
						resourceLoader->PutMesh(geom.meshId, t.filename,t.preScale);
					}
					t.mesh = resourceLoader->GetMesh(geom.meshId);
					geom.mesh = t.mesh;
				}
			}

			for (auto& t : materialParts){
				if (t.id == geom.materialId){
					geom.material = &t.material;
				}
			}
		}

		return true;
	}

	int Scene::KeyWordIndex(const std::string& s, std::vector<std::string>& keywords) {
		int n = keywords.size();
		for (int i = 0; i < n; i++){
			const std::string& k = keywords[i];
			if (s.compare(k) == 0){
				return i;
			}
		}
		return -1;
	}

	int Scene::KeywordIndex(char* str, std::vector<std::string>& keywords) {
		int n = keywords.size();
		for (int i = 0; i < n; i++){
			const std::string& pk = keywords[i];
			const char* pks = pk.c_str();
			if (strcmp(str, pks) == 0){
				return i;
			}
		}
		return -1;
	}

	void Scene::ShowError(std::string& expected, int lineNo) {
		int commentsSofarUptoThisLine = 0;
		if (lineNo < commentsSoFarAtEachLine.size()){
			commentsSofarUptoThisLine = commentsSoFarAtEachLine[lineNo];
		}
		std::cout << "expected " << expected << " at line " << lineNo + commentsSofarUptoThisLine << "\n";
	}

	int Scene::ReadStringFromLine(char *buf, int bufLen, std::string& line, int startIndex) 
	{
		int lineLength = line.length();
		int k = startIndex;
		while (k < lineLength && (line[k] == ' ' || line[k] == '\t' || line[k] == '"')){
			++k;
		}
		if (k >= lineLength){
			return lineLength - 1;
		}
		int index = 0;
		do{
			buf[index] = line[k];
			buf[index + 1] = '\0';
			++index;
			++k;
		} while (k < lineLength && index < bufLen - 1 && line[k] != ' ' && line[k] != '\t' &&  line[k] != '"');

		if (k >= lineLength){
			return lineLength - 1;
		}
		else {
			return k;
		}
	}

	void Scene::ReadInt(int &i1, std::string& line, int& marker)
	{
		char buf[256];
		int bufLen = 256;
		marker = ReadStringFromLine(buf, bufLen, line, marker);
		i1 = atoi(buf);
	}

	void Scene::ReadVec3f(n3d::vec3f& v, std::string& line, int& marker) 
	{
		char buf[256];
		int bufLen = 256;
		marker = ReadStringFromLine(buf, bufLen, line, marker);
		v.x = atof(buf);
		marker = ReadStringFromLine(buf, bufLen, line, marker);
		v.y = atof(buf);
		marker = ReadStringFromLine(buf, bufLen, line, marker);
		v.z = atof(buf);
	}

	void Scene::ReadColor3f(n3d::color3f& c, std::string& line, int& marker)
	{
		char buf[256];
		int bufLen = 256;
		marker = ReadStringFromLine(buf, bufLen, line, marker);
		c.b = atof(buf);
		marker = ReadStringFromLine(buf, bufLen, line, marker);
		c.g = atof(buf);
		marker = ReadStringFromLine(buf, bufLen, line, marker);
		c.r = atof(buf);
	}

	void Scene::ReadFloat(float &f1, std::string& line, int& marker)
	{
		char buf[256];
		int bufLen = 256;
		marker = ReadStringFromLine(buf, bufLen, line, marker);
		f1 = atof(buf);
	}

	void Scene::ReadFloat2(n3d::float2 &f2, std::string& line, int& marker)
	{
		char buf[256];
		int bufLen = 256;
		marker = ReadStringFromLine(buf, bufLen, line, marker);
		f2.v[0] = atof(buf);
		marker = ReadStringFromLine(buf, bufLen, line, marker);
		f2.v[1] = atof(buf);
		marker = ReadStringFromLine(buf, bufLen, line, marker);
	}

	void Scene::ReadFloat3(n3d::float3 &f3, std::string& line, int& marker)
	{
		char buf[256];
		int bufLen = 256;
		marker = ReadStringFromLine(buf, bufLen, line, marker);
		f3.v[0] = atof(buf);
		marker = ReadStringFromLine(buf, bufLen, line, marker);
		f3.v[1] = atof(buf);
		marker = ReadStringFromLine(buf, bufLen, line, marker);
		f3.v[2] = atof(buf);
	}

	void Scene::ReadFloat4(n3d::float4 &f4, std::string& line, int& marker)
	{
		char buf[256];
		int bufLen = 256;
		marker = ReadStringFromLine(buf, bufLen, line, marker);
		f4.v[0] = atof(buf);
		marker = ReadStringFromLine(buf, bufLen, line, marker);
		f4.v[1] = atof(buf);
		marker = ReadStringFromLine(buf, bufLen, line, marker);
		f4.v[2] = atof(buf);
		marker = ReadStringFromLine(buf, bufLen, line, marker);
		f4.v[3] = atof(buf);
	}

	void Scene::ReadFloat5(n3d::float5 &f5, std::string& line, int& marker)
	{
		char buf[256];
		int bufLen = 256;
		marker = ReadStringFromLine(buf, bufLen, line, marker);
		f5.v[0] = atof(buf);
		marker = ReadStringFromLine(buf, bufLen, line, marker);
		f5.v[1] = atof(buf);
		marker = ReadStringFromLine(buf, bufLen, line, marker);
		f5.v[2] = atof(buf);
		marker = ReadStringFromLine(buf, bufLen, line, marker);
		f5.v[3] = atof(buf);
		marker = ReadStringFromLine(buf, bufLen, line, marker);
		f5.v[4] = atof(buf);
	}

	void Scene::ReadFloat6(n3d::float6 &f6, std::string& line, int& marker)
	{
		char buf[256];
		int bufLen = 256;
		marker = ReadStringFromLine(buf, bufLen, line, marker);
		f6.v[0] = atof(buf);
		marker = ReadStringFromLine(buf, bufLen, line, marker);
		f6.v[1] = atof(buf);
		marker = ReadStringFromLine(buf, bufLen, line, marker);
		f6.v[2] = atof(buf);
		marker = ReadStringFromLine(buf, bufLen, line, marker);
		f6.v[3] = atof(buf);
		marker = ReadStringFromLine(buf, bufLen, line, marker);
		f6.v[4] = atof(buf);
		marker = ReadStringFromLine(buf, bufLen, line, marker);
		f6.v[5] = atof(buf);
	}

	bool Scene::DoesLineStartWithExpectedKeyword(
		std::string& line, int lineNo,
		int& marker, 
		std::vector<std::string>& keywords, 
		int expectedKeywordIndex) 
	{
		char buf[256];
		int bufLen = 256;
		marker = ReadStringFromLine(buf, bufLen, line, 0);
		//std::cout << buf << "\n";
		int ki = KeywordIndex(buf, keywords);
		if (ki != expectedKeywordIndex){
			ShowError(keywords[expectedKeywordIndex], lineNo);
			return false;
		}
		return true;
	}

	bool Scene::ParseProjectionPart()
	{
		char buf[256];
		int n = projectionLines.size();

		for (int i = 0; i < n; i++){
			const IndexPair p = projectionLines[i];
			int marker = 0;
			int ki = 0;
			int id = 0;
			float fovy = 0;
			float near = 0;
			float far = 0;
			float left = 0;
			float right = 0;
			float bottom = 0;
			float top = 0;
			float orthoNear = 0;
			float orthoFar = 0;
			n3d::float6 f6;
			n3d::float3 f3;

			for (int j = p.firstIndex, order=0; j <= p.secondIndex; j++, order++) 
			{
				std::string& line = fileLines[j];
				switch (order) {
				case 0:
					marker = 0;
					if (
						!DoesLineStartWithExpectedKeyword(line, j + 1, marker,
							projectionKeywords,
							order)
						){
						return false;
					}
					ReadInt(id, line, marker);
					break;
				case 1:
					marker = 0;
					if (
						!DoesLineStartWithExpectedKeyword(line, j + 1, marker,
						projectionKeywords,
						order)
						) 
					{
						return false;
					}
					ReadFloat3(f3, line, marker);
					fovy = f3.v[0];
					near = f3.v[1];
					far = f3.v[2];
					break;
				case 2:
					marker = 0;
					if (
						!DoesLineStartWithExpectedKeyword(line, j + 1, marker,
						projectionKeywords,
						order)
						)
					{
						return false;
					}
					ReadFloat6(f6, line, marker);
					left = f6.v[0];
					right = f6.v[1];
					bottom = f6.v[2];
					top = f6.v[3];
					orthoNear = f6.v[4];
					orthoFar = f6.v[5];
					break;
				}
			}
			ProjectionPart pp;
			pp.id = id;
			pp.fovy = fovy;
			pp.near = near;
			pp.far = far;
			pp.left = left;
			pp.right = right;
			pp.bottom = bottom;
			pp.orthoNear = orthoNear;
			pp.orthoFar = orthoFar;
			projectionParts.push_back(pp);
			//std::cout << "\n";
			//std::cout << "\nid " << id;

			//std::cout << "\nfovy " << fovy;
			//std::cout << "\nnear " << near;
			//std::cout << "\nfar " << far;

			//std::cout << "\nleft " << left;
			//std::cout << "\nright " << right;
			//std::cout << "\nbottom " << bottom;
			//std::cout << "\ntop " << top;
			//std::cout << "\nonear " << orthoNear;
			//std::cout << "\nofar " << orthoFar;

		}
		return true;
	}

	bool Scene::ParseViewPart() {
		char buf[256];
		int n = viewLines.size();
		for (int i = 0; i < n; i++) {
			const IndexPair p = viewLines[i];
			int marker = 0;
			int ki = 0;
			int id = 0;
			n3d::vec3f eye;
			n3d::vec3f at;
			n3d::vec3f up;
			for (int j = p.firstIndex, order = 0; j <= p.secondIndex; j++, order++)
			{
				std::string& line = fileLines[j];
				switch (order) {
				case 0:
					marker = 0;
					if (
						!DoesLineStartWithExpectedKeyword(line, j + 1, marker,
						viewKeywords,
						order)
						)
					{
						return false;
					}
					ReadInt(id, line, marker);
					break;
				case 1:
					marker = 0;
					if (
						!DoesLineStartWithExpectedKeyword(line, j + 1, marker,
						viewKeywords,
						order)
						)
					{
						return false;
					}
					ReadVec3f(eye, line, marker);
					break;
				case 2:
					marker = 0;
					if (
						!DoesLineStartWithExpectedKeyword(line, j + 1, marker,
						viewKeywords,
						order)
						)
					{
						return false;
					}
					ReadVec3f(at, line, marker);
					break;
				case 3:
					marker = 0;
					if (
						!DoesLineStartWithExpectedKeyword(line, j + 1, marker,
						viewKeywords,
						order)
						)
					{
						return false;
					}
					//marker = ReadStringFromLine(buf, 256, line, marker);
					ReadVec3f(up, line, marker);
					break;
				}
			}
			//std::cout << "id " << id << "\n";
			//std::cout << "eye " << eye.x << "," << eye.y <<","<<eye.z << "\n";
			//std::cout << "at " << at.x << "," << at.y << "," << at.z << "\n";
			//std::cout << "up " << up.x << "," << up.y << "," << up.z << "\n";
			ViewPart vp;
			vp.id = id;
			vp.eye = eye;
			vp.at = at;
			vp.up = up;
			viewParts.push_back(vp);
		}
		return true;
	}

	bool Scene::ParseMeshPart() {
		char buf[256];
		int n = meshLines.size();
		for (int i = 0; i < n; i++) {
			const IndexPair p = meshLines[i];
			int marker = 0;
			int ki = 0;
			int id = 0;
			std::string filename;
			n3d::vec3f preScale;
			for (int j = p.firstIndex, order = 0; j <= p.secondIndex; j++, order++){
				std::string& line = fileLines[j];
				switch (order) {
				case 0:
					marker = 0;
					if (
						!DoesLineStartWithExpectedKeyword(line, j + 1, marker,
						meshKeywords,
						order)
						)
					{
						return false;
					}
					ReadInt(id, line, marker);
					break;
				case 1:
					marker = 0;
					if (
						!DoesLineStartWithExpectedKeyword(line, j + 1, marker,
						meshKeywords,
						order)
						)
					{
						return false;
					}
					marker = ReadStringFromLine(buf, 256, line, marker);
					filename = std::string(buf);
					break;
				case 2:
					marker = 0;
					if (
						!DoesLineStartWithExpectedKeyword(line, j + 1, marker,
						meshKeywords,
						order)
						)
					{
						return false;
					}
					ReadVec3f(preScale, line, marker);
					break;
				}
			}
			std::cout << "id " << id << "\n";
			std::cout << "filename " << filename << "\n";
			MeshPart mp;
			mp.filename = filename;
			///mp.mesh = new Mesh();
			//mp.mesh->LoadObjectFile(filename.c_str());
			mp.mesh = nullptr;
			mp.id = id;
			mp.preScale = preScale;
			meshParts.push_back(mp);
		}
		return true;
	}

	bool Scene::ParseTexturePart() {
		char buf[256];
		int n = textureLines.size();
		for (int i = 0; i < n; i++) {
			const IndexPair p = textureLines[i];
			int marker = 0;
			int ki = 0;
			int id = 0;
			std::string filename;
			for (int j = p.firstIndex, order = 0; j <= p.secondIndex; j++, order++){
				std::string& line = fileLines[j];
				switch (order) {
				case 0:
					marker = 0;
					if (
						!DoesLineStartWithExpectedKeyword(line, j + 1, marker,
						textureKeywords,
						order)
						)
					{
						return false;
					}
					ReadInt(id, line, marker);
					break;
				case 1:
					marker = 0;
					if (
						!DoesLineStartWithExpectedKeyword(line, j + 1, marker,
						textureKeywords,
						order)
						)
					{
						return false;
					}
					marker = ReadStringFromLine(buf, 256, line, marker);
					filename = std::string(buf);
					break;
				}
			}
			TexturePart tp;
			tp.filename = filename;
			//tp.image = new spn::Image();
			//tp.image->CreateFromPng(filename.c_str());
			tp.image = nullptr;
			tp.id = id;
			textureParts.push_back(tp);
			std::cout << "id " << id << "\n";
			std::cout << "filename " << filename << "\n";
		}
		return true;
	}

	bool Scene::ParseMaterialPart(){
		char buf[256];
		int n = materialsLines.size();
		for (int i = 0; i < n; i++) {
			const IndexPair p = materialsLines[i];
			int marker = 0;
			int ki = 0;
			int id = 0;
			n3d::vec3f ambient;
			n3d::vec3f diffuse;
			for (int j = p.firstIndex, order = 0; j <= p.secondIndex; j++, order++) {
				std::string& line = fileLines[j];
				switch (order) {
				case 0:
					marker = 0;
					if (
						!DoesLineStartWithExpectedKeyword(line, j + 1, marker,
						materialKeywords,
						order)
						)
					{
						return false;
					}
					ReadInt(id, line, marker);
					break;
				case 1:
					marker = 0;
					if (
						!DoesLineStartWithExpectedKeyword(line, j + 1, marker,
						materialKeywords,
						order)
						)
					{
						return false;
					}
					ReadVec3f(ambient, line, marker);
					vec3f_reverse_order(&ambient, &ambient);
					break;
				case 2:
					marker = 0;
					if (
						!DoesLineStartWithExpectedKeyword(line, j + 1, marker,
						materialKeywords,
						order)
						)
					{
						return false;
					}
					ReadVec3f(diffuse, line, marker);
					vec3f_reverse_order(&diffuse, &diffuse);
					break;

				}
			}
			MaterialPart mp;
			mp.id = id;
			mp.material.ambient = ambient;
			mp.material.diffuse = diffuse;
			materialParts.push_back(mp);
			//std::cout << "id " << id << "\n";
			//std::cout << "ambient " << ambient.z << " " << ambient.y << " " << ambient.x << "\n";
			//std::cout << "diffuse " << diffuse.z << " " << diffuse.y << " " << diffuse.x << "\n";
		}
		return true;
	}

	bool Scene::ParsePointLightPart(){
		char buf[256];
		int n = pointLightLines.size();
		for (int i = 0; i < n; i++) {
			const IndexPair p = pointLightLines[i];
			int marker = 0;
			int ki = 0;
			int id = 0;
			n3d::color3f ambience;
			n3d::color3f diffuse;
			n3d::vec3f trans;
			for (int j = p.firstIndex, order = 0; j <= p.secondIndex; j++, order++) {
				std::string& line = fileLines[j];
				switch (order) {
				case 0:
					marker = 0;
					if (
						!DoesLineStartWithExpectedKeyword(line, j + 1, marker,
						pointLightKeywords,
						order)
						)
					{
						return false;
					}
					ReadInt(id, line, marker);
					break;
				case 1:
					marker = 0;
					if (
						!DoesLineStartWithExpectedKeyword(line, j + 1, marker,
						pointLightKeywords,
						order)
						)
					{
						return false;
					}
					ReadColor3f(ambience, line, marker);
					color3f_reverse_order(&ambience, &ambience);
					break;
				case 2:
					marker = 0;
					if (
						!DoesLineStartWithExpectedKeyword(line, j + 1, marker,
						pointLightKeywords,
						order)
						)
					{
						return false;
					}
					ReadColor3f(diffuse, line, marker);
					color3f_reverse_order(&diffuse, &diffuse);
					break;
				case 3:
					marker = 0;
					if (
						!DoesLineStartWithExpectedKeyword(line, j + 1, marker,
						pointLightKeywords,
						order)
						)
					{
						return false;
					}
					ReadVec3f(trans, line, marker);
					break;
				}
			}
			PointLightPart plp;
			plp.id = id;
			plp.pointLight.ambience = ambience;
			plp.pointLight.diffuse = diffuse;
			vec4f location;
			vec4f_make_point_from_vec3f(&location, &trans);
			plp.pointLight.worldSpaceLocation = location;
			pointLightParts.push_back(plp);

			//std::cout << "id " << id << "\n";
			//std::cout << "ambient " << ambient.z << " " << ambient.y << " " << ambient.x << "\n";
			//std::cout << "diffuse " << diffuse.z << " " << diffuse.y << " " << diffuse.x << "\n";
			//std::cout << "trans " << trans.x << " " << trans.y << " " << trans.z << "\n";
		}
		return true;
	}

	bool Scene::ParseGeometricObjectPart(){
		char buf[256];
		int n = geometricObjectLines.size();
		for (int i = 0; i < n; i++) {
			const IndexPair p = geometricObjectLines[i];
			int marker = 0;
			int ki = 0;
			int id = 0;
			int mesh_id = 0;
			int texture_id = 0;
			int material_id = 0;
			n3d::vec3f scale;
			n3d::vec3f rot;
			n3d::vec3f trans;
			int parent_id;
			for (int j = p.firstIndex, order = 0; j <= p.secondIndex; j++, order++) {
				std::string& line = fileLines[j];
				switch (order) {
				case 0:
					marker = 0;
					if (
						!DoesLineStartWithExpectedKeyword(line, j + 1, marker,
						geometricObjectKeywords,
						order)
						)
					{
						return false;
					}
					ReadInt(id, line, marker);
					break;
				case 1:
					marker = 0;
					if (
						!DoesLineStartWithExpectedKeyword(line, j + 1, marker,
						geometricObjectKeywords,
						order)
						)
					{
						return false;
					}
					ReadInt(mesh_id, line, marker);
					break;
				case 2:
					marker = 0;
					if (
						!DoesLineStartWithExpectedKeyword(line, j + 1, marker,
						geometricObjectKeywords,
						order)
						)
					{
						return false;
					}
					ReadInt(texture_id, line, marker);
					break;
				case 3:
					marker = 0;
					if (
						!DoesLineStartWithExpectedKeyword(line, j + 1, marker,
						geometricObjectKeywords,
						order)
						)
					{
						return false;
					}
					ReadInt(material_id, line, marker);
					break;
				case 4:
					marker = 0;
					if (
						!DoesLineStartWithExpectedKeyword(line, j + 1, marker,
						geometricObjectKeywords,
						order)
						)
					{
						return false;
					}
					ReadVec3f(scale, line, marker);
					break;
				case 5:
					marker = 0;
					if (
						!DoesLineStartWithExpectedKeyword(line, j + 1, marker,
						geometricObjectKeywords,
						order)
						)
					{
						return false;
					}
					ReadVec3f(rot, line, marker);
					break;

				case 6:
					marker = 0;
					if (
						!DoesLineStartWithExpectedKeyword(line, j + 1, marker,
						geometricObjectKeywords,
						order)
						)
					{
						return false;
					}
					ReadVec3f(trans, line, marker);
					break;

				case 7:
					marker = 0;
					if (
						!DoesLineStartWithExpectedKeyword(line, j + 1, marker,
						geometricObjectKeywords,
						order)
						)
					{
						return false;
					}
					ReadInt(parent_id, line, marker);
					break;
				}
			}
			GeometricObjectPart gop;
			gop.id = id;
			gop.parentId = parent_id;
			gop.materialId = material_id;
			gop.textureId = texture_id;
			gop.meshId = mesh_id;
			gop.localScale = scale;
			gop.localRotation = rot;
			gop.localTranslation = trans;
			n3d::mat4x4 t, r, xr,yr,zr,s,temp;
			mat4x4_set_scale(s, scale.x, scale.y, scale.z);
			mat4x4_set_translation(t, trans.x, trans.y, trans.z);
			mat4x4_set_roation_x(xr, rot.x);
			mat4x4_set_roation_y(yr, rot.y);
			mat4x4_set_roation_z(zr, rot.z);
			mat4x4_multiply(r, yr, xr);
			mat4x4_multiply(r, r, zr);
			mat4x4_multiply(temp, t, r);
			mat4x4_multiply(temp, temp, s);
			memcpy(&(gop.modelMat)[0], &temp[0], 16 * sizeof(float));
			geometricObjectParts.push_back(gop);
			//std::cout << "id " << id << "\n";
			//std::cout << "mes id " << mesh_id << "\n";
			//std::cout << "texture id " << texture_id << "\n";
			//std::cout << "material id " << material_id << "\n";
			//std::cout << "scale " << scale.x << " " << scale.y << " " << scale.z << "\n";
			//std::cout << "rot " << rot.x << " " << rot.y << " " << rot.z << "\n";
			//std::cout << "trans " << trans.x << " " << trans.y << " " << trans.z << "\n";
			//std::cout << "parent id " << parent_id << "\n";
		}
		return true;
	}

	bool Scene::ParseInitializerPart(){
		char buf[256];
		int n = initializerLines.size();
		for (int i = 0; i < n; i++) {
			const IndexPair p = initializerLines[i];
			int marker = 0;
			int ki = 0;
			int projection_id = 0;
			int view_id = 0;
			int point_light_id = 0;
			
			for (int j = p.firstIndex, order = 0; j <= p.secondIndex; j++, order++){
				std::string& line = fileLines[j];
				switch (order) {
				case 0:
					marker = 0;
					if (
						!DoesLineStartWithExpectedKeyword(line, j + 1, marker,
						initializerKeywords,
						order)
						)
					{
						return false;
					}
					ReadInt(projection_id, line, marker);
					break;
				case 1:
					marker = 0;
					if (
						!DoesLineStartWithExpectedKeyword(line, j + 1, marker,
						initializerKeywords,
						order)
						)
					{
						return false;
					}
					ReadInt(view_id, line, marker);
					break;
				case 2:
					marker = 0;
					if (
						!DoesLineStartWithExpectedKeyword(line, j + 1, marker,
						initializerKeywords,
						order)
						)
					{
						return false;
					}
					ReadInt(point_light_id, line, marker);
					break;
				}
			}
			InitializerPart ip;
			ip.pointLightId = point_light_id;
			ip.projectionId = projection_id;
			ip.viewId = view_id;
			initializerParts.push_back(ip);
			//std::cout << "projection id " << projection_id << "\n";
			//std::cout << "view id " << view_id << "\n";
			//std::cout << "point light id " << point_light_id << "\n";
		}
		return true;
	}

	void Scene::GetInitializedParts(ProjectionPart& projectionPart,
		ViewPart& viewPart,
		PointLightPart &pointLightPart
		)
	{
		InitializerPart ip = initializerParts[0];
		for (auto& pl : pointLightParts){
			if (pl.id == ip.pointLightId){
				pointLightPart = pl;
				break;
			}
		}
		for (auto& vp : viewParts){
			if (vp.id == ip.viewId){
				viewPart = vp;
				break;
			}
		}

		for (auto& pp: projectionParts){
			if (pp.id == ip.projectionId){
				projectionPart = pp;
				break;
			}
		}
	}
}