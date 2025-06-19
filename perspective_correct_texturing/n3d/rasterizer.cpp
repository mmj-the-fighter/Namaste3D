#include <iostream>
#include "rasterizer.h"
#include "clipper.hpp"
#include "utils.hpp"

#define SPLITTRIANGLESWHILECLIPPING

namespace n3d
{
	Rasterizer::Rasterizer() :canvas(nullptr){
		inputTriList.reserve(10);
		clippedTriList.reserve(512);
		InitRandom();
	}

	Rasterizer::~Rasterizer(){
	}

	//Based on 
	// https://jtsorlinis.github.io/rendering-tutorial/
	// https://ieeexplore.ieee.org/document/8891846
	void Rasterizer::RasterizeVerticesPerspective(SwrVertex *pv0, SwrVertex *pv1, SwrVertex *pv2) {
		vec4f A = pv0->position;
		vec4f B = pv1->position;
		vec4f C = pv2->position;

		texcoord2f tcA = pv0->texcoord;
		texcoord2f tcB = pv1->texcoord;
		texcoord2f tcC = pv2->texcoord;




		const float ABC = EdgeFunction(A, B, C);
		
		if (ABC >= 0) {
			return;
		}

		const float oneOverABC = 1 / ABC;

		tcA.u *= A.w;
		tcA.v *= A.w;

		tcB.u *= B.w;
		tcB.v *= B.w;

		tcC.u *= C.w;
		tcC.v *= C.w;


		int px = 0;
		int py = 0;

		// Get the bounding box of the triangle
		const float minX = MinOfThree(A.x, B.x, C.x);
		const float minY = MinOfThree(A.y, B.y, C.y);
		const float maxX = MaxOfThree(A.x, B.x, C.x);
		const float maxY = MaxOfThree(A.y, B.y, C.y);

		

		// Loop through all the pixels of the bounding box
		for (py = minY; py < maxY; py++) {
			for (px = minX; px < maxX; px++) {
				vec4f P;
				P.x = px;
				P.y = py;

				const float BCP = EdgeFunction(B, C, P);
				if (BCP > 0){
					continue;
				}
				const float CAP = EdgeFunction(C, A, P);
				if (CAP > 0){
					continue;
				}

				const float weightA = BCP * oneOverABC;
				const float weightB = CAP * oneOverABC;
				const float weightC = 1 - weightA - weightB;
				if (weightC < 0) {
					continue;
				}
				float interpolatedOneOverW = (A.w*weightA + B.w*weightB + C.w*weightC);
				float depth = 1 / interpolatedOneOverW;

				float interpolatedU = tcA.u * weightA + tcB.u * weightB + tcC.u * weightC;
				float interpolatedV = tcA.v * weightA + tcB.v * weightB + tcC.v * weightC;
				int texureImageX = 
					interpolatedU  
					* oneMinusTextureCanvasWidth 
					* depth;
				int texureImageY = 
					interpolatedV
					* oneMinusTextureCanvasHeight
					* depth;
				texureImageY = textureCanvasHeight - texureImageY;

				if (texureImageX < 0) {
					texureImageX = 0;
				}
				else if (texureImageX > oneMinusTextureCanvasWidth){
					texureImageX = oneMinusTextureCanvasWidth;
				}

				if (texureImageY < 0) {
					texureImageY = 0;
				}
				else if (texureImageY > oneMinusTextureCanvasHeight) {
					texureImageY = oneMinusTextureCanvasHeight;
				}

				unsigned char* texel =
					textureCanvasPixels
					+ textureCanvasPitch * texureImageY
					+ texureImageX * 4;
								
				unsigned char* loc =
					canvasPixels
					+ canvasPitch * py
					+ px * 4;
				*loc++ = *texel;
				++texel;
				*loc++ = *texel;
				++texel;
				*loc++ = *texel;
				++texel;
				*loc = *texel;
			}
		}

	}



	//Based on 
	// https://jtsorlinis.github.io/rendering-tutorial/
	void Rasterizer::RasterizeVerticesOrtho(SwrVertex *pv0, SwrVertex *pv1, SwrVertex *pv2) {
		vec4f A = pv0->position;
		vec4f B = pv1->position;
		vec4f C = pv2->position;

		texcoord2f tcA = pv0->texcoord;
		texcoord2f tcB = pv1->texcoord;
		texcoord2f tcC = pv2->texcoord;

		const float ABC = EdgeFunction(A, B, C);
		
		if (ABC >= 0) {
			return;
		}

		const float oneOverABC = 1 / ABC;

		int px = 0;
		int py = 0;

		// Get the bounding box of the triangle
		const float minX = MinOfThree(A.x, B.x, C.x);
		const float minY = MinOfThree(A.y, B.y, C.y);
		const float maxX = MaxOfThree(A.x, B.x, C.x);
		const float maxY = MaxOfThree(A.y, B.y, C.y);



		// Loop through all the pixels of the bounding box
		for (py = minY; py < maxY; py++) {
			for (px = minX; px < maxX; px++) {
				vec4f P;
				P.x = px;
				P.y = py;

				const float BCP = EdgeFunction(B, C, P);
				if (BCP > 0){
					continue;
				}
				const float CAP = EdgeFunction(C, A, P);
				if (CAP > 0){
					continue;
				}

				const float weightA = BCP * oneOverABC;
				const float weightB = CAP * oneOverABC;
				const float weightC = 1 - weightA - weightB;
				if (weightC < 0) {
					continue;
				}
				float interpolatedU = tcA.u * weightA + tcB.u * weightB + tcC.u * weightC;
				float interpolatedV = tcA.v * weightA + tcB.v * weightB + tcC.v * weightC;
				int texureImageX = interpolatedU * oneMinusTextureCanvasWidth;
				int texureImageY = interpolatedV * oneMinusTextureCanvasWidth;
				texureImageY = textureCanvasHeight - texureImageY;
				if (texureImageX < 0) {
					texureImageX = 0;
				}
				else if (texureImageX > oneMinusTextureCanvasWidth){
					texureImageX = oneMinusTextureCanvasWidth;
				}

				if (texureImageY < 0) {
					texureImageY = 0;
				}
				else if (texureImageY > oneMinusTextureCanvasHeight) {
					texureImageY = oneMinusTextureCanvasHeight;
				}
				unsigned char* texel =
					textureCanvasPixels
					+ textureCanvasPitch * texureImageY
					+ texureImageX * 4;

				unsigned char* loc =
					canvasPixels
					+ canvasPitch * py
					+ px * 4;
				*loc++ = *texel;
				++texel;
				*loc++ = *texel;
				++texel;
				*loc++ = *texel;
				++texel;
				*loc = *texel;
			}
		}

	}


	void Rasterizer::RenderMesh(Mesh& mesh) {
		camera->Sync(matView, matProjection);
		mat4x4 vm;
		mat4x4_multiply(vm, matView, matModel);
		mat4x4 pvm;
		mat4x4_multiply(pvm, matProjection, vm);

		int i;
		int n = mesh.GetNumOfTriangles() * 3;
		for (i = 0; i < n; i+=3) {

			SwrVertex vertexA;
			vec4f_make_point_from_vec3f(&vertexA.position, &mesh.vertices[i].position);
			vertexA.texcoord = mesh.vertices[i].uv;

			SwrVertex vertexB;
			vec4f_make_point_from_vec3f(&vertexB.position, &mesh.vertices[i+1].position);
			vertexB.texcoord = mesh.vertices[i + 1].uv;

			SwrVertex vertexC;
			vec4f_make_point_from_vec3f(&vertexC.position, &mesh.vertices[i + 2].position);
			vertexC.texcoord = mesh.vertices[i + 2].uv;
			
			mat4x4_transform(&vertexA.position, pvm);
			mat4x4_transform(&vertexB.position, pvm);
			mat4x4_transform(&vertexC.position, pvm);

			SwrTriangle tri;
			tri.a = vertexA;
			tri.b = vertexB;
			tri.c = vertexC;

#ifdef SPLITTRIANGLESWHILECLIPPING
			inputTriList.clear();
			clippedTriList.clear();
			inputTriList.emplace_back(tri);
			int clippedTrianglesSize;
			clippedTriList =
				clipper.ClipTriangleListWithFrustum(inputTriList);
#else
			clippedTriList.clear();
			if ( !clipper.IsTriangleFullyInsideFrustum(tri) ){
				continue;
			}
			clippedTriList.emplace_back(tri);
#endif
			for (size_t i = 0; i < clippedTriList.size(); ++i)
			{
				SwrTriangle tri = clippedTriList[i];
				vec4f& pointA = tri.a.position;
				vec4f& pointB = tri.b.position;
				vec4f& pointC = tri.c.position;

				float s = 1 / pointA.w;
				pointA.x *= s;
				pointA.y *= s;
				pointA.z *= s;
				pointA.w = 1.0;
				mat4x4_transform(&pointA, matViewport);
				pointA.w = s;

				s = 1 / pointB.w;
				pointB.x *= s;
				pointB.y *= s;
				pointB.z *= s;
				pointB.w = 1.0;
				mat4x4_transform(&pointB, matViewport);
				pointB.w = s;

				s = 1 / pointC.w;
				pointC.x *= s;
				pointC.y *= s;
				pointC.z *= s;
				pointC.w = 1.0;
				mat4x4_transform(&pointC, matViewport);
				pointC.w = s;
				
				ClampWithInCanvasWidthAndHeight(pointA);
				ClampWithInCanvasWidthAndHeight(pointB);
				ClampWithInCanvasWidthAndHeight(pointC);

				if (camera->IsPerspective()) {
					RasterizeVerticesPerspective(&tri.a, &tri.b, &tri.c);
				}
				else{
					RasterizeVerticesOrtho(&tri.a, &tri.b, &tri.c);
				}
				
				
			}
		}
	}
}