#include <iostream>
#include "rasterizer.h"
#include "clipper.hpp"
#include "utils.hpp"

#define SPLITTRIANGLESWHILECLIPPING

namespace n3d
{
	Rasterizer::Rasterizer() :
		canvas(nullptr), 
		depthBuffer(nullptr), 
		pointLight(nullptr),
		material(nullptr){
		inputTriList.reserve(10);
		clippedTriList.reserve(512);
	}

	Rasterizer::~Rasterizer(){
		if (nullptr != depthBuffer){
			delete[] depthBuffer;
			depthBuffer = nullptr;
		}
	}

	void Rasterizer::CreateDepthBuffer() {
		if (nullptr != depthBuffer){
			delete[] depthBuffer;
			depthBuffer = nullptr;
		}
		depthBuffer = new float[canvas->GetNumOfPixels()];

		depthImage.CreateSolidColorBlockImage(
			canvasWidth, canvasHeight, 
			0, 0, 0, 255);
		depthImagePixels = 
			depthImage.GetCanvas()->GetPixelBuffer();
	}

	void Rasterizer::ClearDepthBuffer() {
		bool isPerspective = camera->IsPerspective();
		int n = canvas->GetNumOfPixels();
		if (isPerspective) {
			for (int i = 0; i < n; i++) {
				depthBuffer[i] = 0;
			}
		}
		else{
			for (int i = 0; i < n; i++) {
				depthBuffer[i] = std::numeric_limits<float>::infinity();
			}
		}

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

		color3f colorA = pv0->color;
		color3f colorB = pv1->color;
		color3f colorC = pv2->color;


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

		colorA.b *= A.w;
		colorA.g *= A.w;
		colorA.r *= A.w;

		colorB.b *= B.w;
		colorB.g *= B.w;
		colorB.r *= B.w;

		colorC.b *= C.w;
		colorC.g *= C.w;
		colorC.r *= C.w;


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

				// do inverse w-buffering
				int depthBufferIndex = canvasWidth * py + px;
				if (interpolatedOneOverW < depthBuffer[depthBufferIndex]){
					continue;
				}
				depthBuffer[depthBufferIndex] = interpolatedOneOverW;

				float depth = 1 / interpolatedOneOverW;

				float blue = colorA.b * weightA + colorB.b * weightB + colorC.b * weightC;
				float green = colorA.g * weightA + colorB.g * weightB + colorC.g * weightC;
				float red = colorA.r * weightA + colorB.r * weightB + colorC.r * weightC;
				blue *= depth;
				green *= depth;
				red *= depth;

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

				float texelBlue = *texel++;
				float texelGreen = *texel++;
				float texelRed = *texel++;
				float texelAlpha = *texel;

				blue += texelBlue;
				green += texelGreen;
				red += texelRed;

				unsigned char* loc =
					canvasPixels
					+ canvasPitch * py
					+ px * 4;
				
				if (blue > 255) blue = 255;
				if (green > 255) green = 255;
				if (red > 255) red = 255;
				if (texelAlpha > 255) texelAlpha = 255;

				*loc++ = blue;
				*loc++ = green;
				*loc++ = red;
				*loc = texelAlpha;
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

		color3f colorA = pv0->color;
		color3f colorB = pv1->color;
		color3f colorC = pv2->color;

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


				//do normal z-buffering
				double interpolatedZ =
					A.z  * weightA +
					B.z * weightB +
					C.z * weightC;
				int depthBufferIndex = canvasWidth*py + px;
				if (interpolatedZ >= depthBuffer[depthBufferIndex]){
					continue;
				}
				depthBuffer[depthBufferIndex] = interpolatedZ;
				float blue = colorA.b * weightA +
					colorB.b * weightB +
					colorC.b * weightC;
				float green = colorA.g * weightA +
					colorB.g * weightB +
					colorC.g * weightC;
				float red = colorA.r * weightA +
					colorB.r * weightB +
					colorC.r * weightC;
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

				float texelBlue = *texel++;
				float texelGreen = *texel++;
				float texelRed = *texel++;
				float texelAlpha = *texel;

				blue += texelBlue;
				green += texelGreen;
				red += texelRed;

				if (blue > 255) blue = 255;
				if (green > 255) green = 255;
				if (red > 255) red = 255;
				if (texelAlpha > 255) texelAlpha = 255;

				unsigned char* loc =
					canvasPixels
					+ canvasPitch * py
					+ px * 4;
				*loc++ = blue;
				*loc++ = green;
				*loc++ = red;
				*loc = texelAlpha;
			}
		}

	}

	void Rasterizer::RenderDepth() {
		int n = depthImage.GetCanvas()->GetNumOfPixels();
		if (camera->IsPerspective()){
			int pixelNo = 0;
			for (int i = 0; i < n * 4; i+=4) {
				int color = 0;
				float depth = depthBuffer[pixelNo];
				if ( depth != 0){
					color = 255.0f * (1.0f / depth);
				}
				++pixelNo;
				depthImagePixels[i] = color;
				depthImagePixels[i + 1] = color;
				depthImagePixels[i + 2] = color;
				depthImagePixels[i + 3] = 255;
			}
		}
		else{
			int pixelNo = 0;
			for (int i = 0; i < n * 4; i += 4) {
				int color = 0;
				float depth = depthBuffer[pixelNo];
				color = 255.0f * depth;
				++pixelNo;
				depthImagePixels[i] = color;
				depthImagePixels[i + 1] = color;
				depthImagePixels[i + 2] = color;
				depthImagePixels[i + 3] = 255;
			}
		}
		canvas->DrawImage(&depthImage, 0, 0);
	}

	void Rasterizer::RenderMesh(Mesh& mesh) {
		bool isViewModified = camera->IsViewModified();
		camera->Sync(matView, matProjection);		
		
		mat4x4_multiply(matViewModel, matView, matModel);
		mat3x3 upperLeftViewMat;
		mat4x4_extract_upper_left_3x3(upperLeftViewMat, matViewModel);
		mat3x3 inv;
		mat3x3_inverse(inv, upperLeftViewMat);
		mat3x3_transpose(normalMatrix, inv);

		if (isViewModified){
			//transform light pos
			vec4f viewSpaceLightPos4 = pointLight->worldSpaceLocation;
			mat4x4_transform(&viewSpaceLightPos4, matView);
			vec3f viewSpacelightPos3;
			viewSpacelightPos3.x = viewSpaceLightPos4.x;
			viewSpacelightPos3.y = viewSpaceLightPos4.y;
			viewSpacelightPos3.z = viewSpaceLightPos4.z;
			pointLight->viewSpaceLocation = viewSpacelightPos3;
		}

		int i;
		int n = mesh.GetNumOfTriangles() * 3;
		for (i = 0; i < n; i+=3) {
			SwrVertex vertexA;
			MeshVertex meshVertexA = mesh.vertices[i];
			vec4f_make_point_from_vec3f(&vertexA.position, &meshVertexA.position);
			mat4x4_transform(&vertexA.position, matViewModel);
			CalculateVertexColorDueToLighting(&vertexA, &meshVertexA);
			vertexA.texcoord = meshVertexA.uv;

			SwrVertex vertexB;
			MeshVertex meshVertexB = mesh.vertices[i+1];
			vec4f_make_point_from_vec3f(&vertexB.position, &meshVertexB.position);
			mat4x4_transform(&vertexB.position, matViewModel);
			CalculateVertexColorDueToLighting(&vertexB, &meshVertexB);
			vertexB.texcoord = meshVertexB.uv;

			SwrVertex vertexC;
			MeshVertex meshVertexC = mesh.vertices[i + 2];
			vec4f_make_point_from_vec3f(&vertexC.position, &meshVertexC.position);
			mat4x4_transform(&vertexC.position, matViewModel);
			CalculateVertexColorDueToLighting(&vertexC, &meshVertexC);
			vertexC.texcoord = meshVertexC.uv;

			mat4x4_transform(&vertexA.position, matProjection);
			mat4x4_transform(&vertexB.position, matProjection);
			mat4x4_transform(&vertexC.position, matProjection);

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