#include <fstream>
#include <string>
#include <iostream>
#include <sstream>

#include "../../spinach/core/spn_canvas.h"
#include "mesh.h"
#include "utils.hpp"

namespace n3d
{

	Mesh::Mesh(){
	}

	Mesh::Mesh(const Mesh& other){
		int n = other.vertices.size();
		for (int i = 0; i < n; i++) {
			vertices.push_back(other.vertices[i]);
		}
		numTriangles = other.numTriangles;
	}

	Mesh* Mesh::Clone(){
		Mesh* other = new Mesh(*this);
		return other;
	}

		
	void Mesh::LoadObjectFile(const char* filepath){
		std::ifstream file(filepath);
		if (!file.is_open()) {
			std::cerr << "Failed to open mesh file: " <<filepath<< std::endl;
			return;
		}
		std::string line;
		std::vector<vec3f> points;
		std::vector<vec3f> normals;
		std::vector<texcoord2f> uvs;
		
		std::vector<vec3i> indices;
		while (std::getline(file, line)) {
			if (line[0] == 'v' && line[1] == ' '){
				vec3f v;
				ParsePointLine(line, v);
				points.push_back(v);
			}
			else if (line[0] == 'v' && line[1] == 't' && line[2] == ' '){
				texcoord2f uv;
				ParseTextureCoordinateLine(line, uv);
				uvs.push_back(uv);
			}
			else if (line[0] == 'v' && line[1] == 'n' && line[2] == ' '){
				vec3f normal;
				ParseNormalLine(line, normal);
				normals.push_back(normal);
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
			MeshVertex a;
			a.position = points[index0.x - 1];
			a.uv = uvs[index0.y - 1];
			a.normal = normals[index0.z - 1];
			vertices.push_back(a);

			MeshVertex b;
			b.position = points[index1.x - 1];
			b.uv = uvs[index1.y - 1];
			b.normal = normals[index1.z - 1];
			vertices.push_back(b);

			MeshVertex c;
			c.position = points[index2.x - 1];
			c.uv = uvs[index2.y - 1];
			c.normal = normals[index2.z - 1];
			vertices.push_back(c);
			
			
			
			//
			//vertices.push_back(verticesTemp[index1.x - 1]);
			//vertices.push_back(verticesTemp[index2.x - 1]);
		}
	}

	void Mesh::ParseTextureCoordinateLine(const std::string& line, texcoord2f& texcoord)
	{
		int i = 0;
		int lineLength = line.length();
		char c;
		char temp[256];
		int uStart = 0;
		int uEnd = 0;
		int vStart = 0;
		int vEnd = 0;
		if (line[i] == 'v') {
			++i;
		}
		if (line[i] == 't') {
			++i;
		}

		while (line[i] == ' ') {
			++i;
		}

		c = line[i];
		if (c >= '0' && c <= '9' || c == '-'){
			uStart = i;
			uEnd = i;
		}
		while (line[i] != ' ') {
			uEnd = i;
			++i;
		}

		while (line[i] == ' ') {
			++i;
		}

		c = line[i];
		if (c >= '0' && c <= '9' || c == '-'){
			vStart = i;
			vEnd = i;
		}

		while (i < lineLength && line[i] != ' ') {
			vEnd = i;
			++i;
		}

		i = 0;
		for (int j = uStart; j <= uEnd; ++j, ++i){
			temp[i] = line[j];
		}
		temp[i] = '\0';
		texcoord.u = atof(temp);

		i = 0;
		for (int j = vStart; j <= vEnd; ++j, ++i){
			temp[i] = line[j];
		}
		temp[i] = '\0';
		texcoord.v = atof(temp);

	}

	void Mesh::ParseNormalLine(const std::string& line, vec3f& normal){
		int i = 0;
		int lineLength = line.length();
		char c;
		char temp[256];
		int xStart = 0;
		int xEnd = 0;
		int yStart = 0;
		int yEnd = 0;
		int zStart = 0;
		int zEnd = 0;

		if (line[i] == 'v') {
			++i;
		}

		if (line[i] == 'n') {
			++i;
		}

		while (line[i] == ' ') {
			++i;
		}
		//Find x bounds

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
		for (int j = xStart; j <= xEnd; ++j, ++i){
			temp[i] = line[j];
		}
		temp[i] = '\0';
		normal.x = atof(temp);

		i = 0;
		for (int j = yStart; j <= yEnd; ++j, ++i){
			temp[i] = line[j];
		}
		temp[i] = '\0';
		normal.y = atof(temp);

		i = 0;
		for (int j = zStart; j <= zEnd; ++j, ++i){
			temp[i] = line[j];
		}
		temp[i] = '\0';
		normal.z = atof(temp);

		//std::cout << normal.x << ", " << normal.y << ", " << normal.z << "\n";
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