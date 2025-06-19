#include <fstream>
#include <string>
#include <iostream>
#include <sstream>

#include "../../spinach/core/spn_canvas.h"
#include "mesh.h"

namespace n3d
{

	Mesh::Mesh(){}
		
	void Mesh::LoadObjectFile(const char* filepath){
		std::ifstream file(filepath);
		if (!file.is_open()) {
			std::cerr << "Failed to open mesh file: " <<filepath<< std::endl;
			return;
		}
		std::string line;
		vertices.clear();
		while (std::getline(file, line)) {
			if (line[0] == 'v' && line[1] == ' '){
				vec3f v;
				ParsePointLine(line, v);
				MeshVertex mv;
				mv.position = v;
				vertices.push_back(mv);
			}
		}
	}

	void Mesh::ParsePointLine(const std::string& line, vec3f& point){
		int i = 0;
		int lineLength = line.length();
		char c;
		char temp[256];
		int xStart = 0, xEnd = 0;
		int yStart = 0, yEnd = 0;
		int zStart = 0, zEnd = 0;
		if (line[i] == 'v') {
			++i;
		}
		//Find x bounds
		while (line[i] == ' ') {
			++i;
		}

		c = line[i];
		if (c >= '0' && c <= '9' || c == '-'){
			xStart = i;
			xEnd = i;
		}

		while (line[i] != ' ') {
			xEnd = i;
			++i;
		}
		

		//Find y bounds
		while (line[i] == ' ') {
			++i;
		}

		c = line[i];
		if (c >= '0' && c <= '9' || c == '-'){
			yStart = i;
			yEnd = i;
		}

		while (line[i] != ' ') {
			yEnd = i;
			++i;
		}
		


		//Find z bounds
		while (line[i] == ' ') {
			++i;
		}

		c = line[i];
		if (c >= '0' && c <= '9' || c == '-'){
			zStart = i;
			zEnd = i;
		}

		while (i < lineLength && line[i] != ' ') {
			zEnd = i;
			++i;
		}

		i = 0;
		for (int j = xStart; j <= xEnd; ++j,++i){
			temp[i] = line[j];
		}
		temp[i] = '\0';
		point.x = atof(temp);

		i = 0;
		for (int j = yStart; j <= yEnd; ++j, ++i){
			temp[i] = line[j];
		}
		temp[i] = '\0';
		point.y = atof(temp);
		
		i = 0;
		for (int j = zStart; j <= zEnd; ++j, ++i){
			temp[i] = line[j];
		}
		temp[i] = '\0';
		point.z = atof(temp);
	}
}