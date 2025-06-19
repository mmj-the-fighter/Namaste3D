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
		std::vector<MeshVertex> verticesTemp;
		
		std::vector<vec3i> indices;
		while (std::getline(file, line)) {
			if (line[0] == 'v' && line[1] == ' '){
				vec3f v;
				ParsePointLine(line, v);
				MeshVertex mv;
				mv.position = v;
				verticesTemp.push_back(mv);
			}
			else if (line[0] == 'f'){
				vec3i a, b, c;
				ParseTriangleLine(line, a, b, c);
				indices.push_back(a);
				indices.push_back(b);
				indices.push_back(c);
			}
		}

		vertices.clear();
		int indicesSize = indices.size();
		numTriangles = indicesSize / 3;
		for (int k = 0; k < indicesSize; k += 3){
			vec3i& index0 = indices[k];
			vec3i& index1 = indices[k + 1];
			vec3i& index2 = indices[k + 2];
			vertices.push_back(verticesTemp[index0.x - 1]);
			vertices.push_back(verticesTemp[index1.x - 1]);
			vertices.push_back(verticesTemp[index2.x - 1]);
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

	void Mesh::ParseTriangleLine(std::string& line, vec3i& p, vec3i& q, vec3i& r)
	{
		int i = 0;
		char c;
		char temp[256];
		int lineLength = line.length();

		//Replace / with space
		for (i = 0; i < lineLength; i++) {
			if (line[i] == '/'){
				line[i] = ' ';
			}
		}
		i = 0;
		if (line[i] == 'f') {
			++i;
		}

		//point0Index;
		//textureCoordinate0Index;
		//normal0Index;
		
//First set of indices
		//Find point0Index Bounds
		while (line[i] == ' ') {
			++i;
		}
		
		vec2i p0IndexBounds;
		c = line[i];
		if (c >= '0' && c <= '9' || c == '-'){
			p0IndexBounds.x = i;
			p0IndexBounds.y = i;
		}

		while (line[i] != ' ') {
			p0IndexBounds.y = i;
			++i;
		}

		//Find textureCoordinate0Index Bounds
		while (line[i] == ' ') {
			++i;
		}
		
		vec2i t0IndexBounds;
		c = line[i];
		if (c >= '0' && c <= '9' || c == '-'){
			t0IndexBounds.x = i;
			t0IndexBounds.y = i;
		}

		while (line[i] != ' ') {
			t0IndexBounds.y = i;
			++i;
		}

		//Find normal0Index Bounds
		while (line[i] == ' ') {
			++i;
		}

		vec2i n0IndexBounds;
		c = line[i];
		if (c >= '0' && c <= '9' || c == '-'){
			n0IndexBounds.x = i;
			n0IndexBounds.y = i;
		}

		while (line[i] != ' ') {
			n0IndexBounds.y = i;
			++i;
		}
//Second set of indices
		//Find point1Index Bounds
		while (line[i] == ' ') {
			++i;
		}

		vec2i p1IndexBounds;
		c = line[i];
		if (c >= '0' && c <= '9' || c == '-'){
			p1IndexBounds.x = i;
			p1IndexBounds.y = i;
		}

		while (line[i] != ' ') {
			p1IndexBounds.y = i;
			++i;
		}

		//Find textureCoordinate1Index Bounds
		while (line[i] == ' ') {
			++i;
		}

		vec2i t1IndexBounds;
		c = line[i];
		if (c >= '0' && c <= '9' || c == '-'){
			t1IndexBounds.x = i;
			t1IndexBounds.y = i;
		}

		while (line[i] != ' ') {
			t1IndexBounds.y = i;
			++i;
		}

		//Find normal1Index Bounds
		while (line[i] == ' ') {
			++i;
		}

		vec2i n1IndexBounds;
		c = line[i];
		if (c >= '0' && c <= '9' || c == '-'){
			n1IndexBounds.x = i;
			n1IndexBounds.y = i;
		}

		while (line[i] != ' ') {
			n1IndexBounds.y = i;
			++i;
		}
//third set of indices
		//Find point2Index Bounds
		while (line[i] == ' ') {
			++i;
		}

		vec2i p2IndexBounds;
		c = line[i];
		if (c >= '0' && c <= '9' || c == '-'){
			p2IndexBounds.x = i;
			p2IndexBounds.y = i;
		}

		while (line[i] != ' ') {
			p2IndexBounds.y = i;
			++i;
		}

		//Find textureCoordinate2Index Bounds
		while (line[i] == ' ') {
			++i;
		}

		vec2i t2IndexBounds;
		c = line[i];
		if (c >= '0' && c <= '9' || c == '-'){
			t2IndexBounds.x = i;
			t2IndexBounds.y = i;
		}

		while (line[i] != ' ') {
			t2IndexBounds.y = i;
			++i;
		}

		//Find normal2Index Bounds
		while (line[i] == ' ') {
			++i;
		}

		vec2i n2IndexBounds;
		c = line[i];
		if (c >= '0' && c <= '9' || c == '-'){
			n2IndexBounds.x = i;
			n2IndexBounds.y = i;
		}

		while (i < lineLength && line[i] != ' ') {
			n2IndexBounds.y = i;
			++i;
		}

		//first set
		int j;
		for (i = 0,j = p0IndexBounds.x; j <= p0IndexBounds.y; ++i, ++j){
			temp[i] = line[j];
		}
		temp[i] = '\0';
		p.x = atoi(temp);

		for (i = 0, j = t0IndexBounds.x; j <= t0IndexBounds.y; ++i, ++j){
			temp[i] = line[j];
		}
		temp[i] = '\0';
		p.y = atoi(temp);

		for (i = 0, j = n0IndexBounds.x; j <= n0IndexBounds.y; ++i, ++j){
			temp[i] = line[j];
		}
		temp[i] = '\0';
		p.z = atoi(temp);

		//second set
		for (i = 0, j = p1IndexBounds.x; j <= p1IndexBounds.y; ++i, ++j){
			temp[i] = line[j];
		}
		temp[i] = '\0';
		q.x = atoi(temp);

		for (i = 0, j = t1IndexBounds.x; j <= t1IndexBounds.y; ++i, ++j){
			temp[i] = line[j];
		}
		temp[i] = '\0';
		q.y = atoi(temp);

		for (i = 0, j = n1IndexBounds.x; j <= n1IndexBounds.y; ++i, ++j){
			temp[i] = line[j];
		}
		temp[i] = '\0';
		q.z = atoi(temp);

		//third set
		for (i = 0, j = p2IndexBounds.x; j <= p2IndexBounds.y; ++i, ++j){
			temp[i] = line[j];
		}
		temp[i] = '\0';
		r.x = atoi(temp);

		for (i = 0, j = t2IndexBounds.x; j <= t2IndexBounds.y; ++i, ++j){
			temp[i] = line[j];
		}
		temp[i] = '\0';
		r.y = atoi(temp);

		for (i = 0, j = n2IndexBounds.x; j <= n2IndexBounds.y; ++i, ++j){
			temp[i] = line[j];
		}
		temp[i] = '\0';
		r.z = atoi(temp);

		//std::cout << p.x << "," << p.y << "," << p.z << '\t';
		//std::cout << q.x << "," << q.y << "," << q.z << '\t';
		//std::cout << r.x << "," << r.y << "," << r.z << '\n';
		
	}
}