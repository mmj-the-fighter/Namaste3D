#ifndef _CLIPPER_HPP_
#define _CLIPPER_HPP_
#include <vector>
#include <array>
#include <iostream>
#include "vector.hpp"
#include "rasterizer.h"
#include "n3d.h"


/*
Clippper.hpp is derived from project sw3dcpp by Andrew Lim
Github repo:  https://github.com/andrew-lim/sw3dcpp/
License for this file:

MIT License

Copyright (c) 2022 Andrew Lim

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.


*/
/*
Clipping style:
-Sutherland-Hodgman algorithm.
-At clipspace
-With homogeneous coordinates
-Clips with all frustum planes
-Uses edge-by-edge intersection to create new vertices.

Rationale:
After perspective divide by w, the coordinates become normalized:
i.e. for x
-1 <= x <= 1

So therefore at clipspace (before perspective divide),
the following equation  should be satisfied
for x:
-1 <= x/w <=1
or
-w <= x <= w

similarly:
-w <= x <= w
-w <= y <= w
-w <= z <= w

*/

namespace n3d{

	struct FrustumParam{
		int coordinateIndex;
		int planeSign;
	};


	class Clipper
	{
	private:
		inline void Interpolate(SwrVertex &i, SwrVertex a, SwrVertex b, float t) {
			// Interpolate position
			i.position.x = a.position.x + (b.position.x - a.position.x) * t;
			i.position.y = a.position.y + (b.position.y - a.position.y) * t;
			i.position.z = a.position.z + (b.position.z - a.position.z) * t;
			i.position.w = a.position.w + (b.position.w - a.position.w) * t;

			//i.color.b = a.color.b + (b.color.b - a.color.b) * t;
			//i.color.g = a.color.g + (b.color.g - a.color.g) * t;
			//i.color.r = a.color.r + (b.color.r - a.color.r) * t;

			i.texcoord.u = a.texcoord.u + (b.texcoord.u - a.texcoord.u) * t;
			i.texcoord.v = a.texcoord.v + (b.texcoord.v - a.texcoord.v) * t;
		}
		inline float CalculateSignedDistanceRatio(const vec4f& src,
			const vec4f& dst,
			int ixyz,
			int planeSign)
		{
			const float d1 = (src.v[ixyz] - src.w * planeSign);
			const float d2 = (dst.v[ixyz] - dst.w * planeSign);
			return d1 / (d1 - d2);
		}

		inline bool
			ClipTriangleWithFrustumPlane(
			const SwrTriangle& triangle,
			int ixyz,
			int planeSign,
			std::vector<SwrTriangle>& out
			)
		{
			insidePointsCount = 0;
			outsidePointsCount = 0;

			for (int i = 0; i<3; ++i) {
				SwrVertex pt = triangle.v[i];
				const float xyz = pt.position.v[ixyz];
				const float w = pt.position.w;
				const bool outside = 
					(planeSign<0 && xyz<-w) || 
					(planeSign>0 && xyz>w);
				if (outside) {
					outsidePoints[outsidePointsCount] = pt;
					outsideIndices[outsidePointsCount] = i;
					++outsidePointsCount;
				}
				else {
					insidePoints[insidePointsCount] = pt;
					insideIndices[insidePointsCount] = i;
					++insidePointsCount;
				}
			}

			// Triangle is completely inside
			if (3 == insidePointsCount) {
				out.push_back(triangle);
				return false;
			}

			// 2 points outside, create a smaller triangle
			else if (2 == outsidePointsCount && 1 == insidePointsCount) {
				SwrVertex& va = insidePoints[0];
				SwrVertex& vb = outsidePoints[0];
				SwrVertex& vc = outsidePoints[1];
				vec4f& a = va.position; // a is inside
				vec4f& b = vb.position;
				vec4f& c = vc.position;

				//interpolate positions
				int ai = insideIndices[0];
				int bi = outsideIndices[0];
				float bt = CalculateSignedDistanceRatio(b, a, ixyz, planeSign);
				float ct = CalculateSignedDistanceRatio(c, a, ixyz, planeSign);
				SwrVertex vb1;
				Interpolate(vb1, vb, va, bt);
				SwrVertex vc1;
				Interpolate(vc1, vc, va, ct);
				// Preserve winding order
				// B follows A
				if (((ai + 1) % 3) == bi) {
					SwrTriangle t;
					t.a = va;
					t.b = vb1;
					t.c = vc1;
					out.push_back(t);
				}
				// C follows A
				else {
					SwrTriangle t;
					t.a = va;
					t.b = vc1;
					t.c = vb1;
					out.push_back(t);
				}
			}

			// 1 point outside, create 2 smaller triangles
			else if (1 == outsidePointsCount && 2 == insidePointsCount) {
				SwrVertex& va = insidePoints[0];
				SwrVertex& vb = outsidePoints[0];
				SwrVertex& vc = insidePoints[1];
				vec4f& a = va.position; // a is inside
				vec4f& b = vb.position;
				vec4f& c = vc.position; // c is inside

				int ai = insideIndices[0];
				int bi = outsideIndices[0];
				float abt = CalculateSignedDistanceRatio(b, a, ixyz, planeSign);
				float cbt = CalculateSignedDistanceRatio(b, c, ixyz, planeSign);
				SwrVertex va1;
				Interpolate(va1, vb, va, abt);
				SwrVertex vc1;
				Interpolate(vc1, vb, vc, cbt);

				// Preserve winding order
				// B follows A
				if (((ai + 1) % 3) == bi) {
					SwrTriangle t1;
					t1.a = va;
					t1.b = va1;
					t1.c = vc1;
					out.push_back(t1);
					SwrTriangle t2;
					t2.a = va;
					t2.b = vc1;
					t2.c = vc;
					out.push_back(t2);
				}
				// C follows A
				else {
					SwrTriangle t1;
					t1.a = va;
					t1.b = vc;
					t1.c = vc1;
					out.push_back(t1);
					SwrTriangle t2;
					t2.a = va;
					t2.b = vc1;
					t2.c = va1;
					out.push_back(t2);
				}
			}
			return true;
		}


		std::array<SwrVertex, 3> insidePoints;
		std::array<SwrVertex, 3> outsidePoints;
		std::array<int, 3> insideIndices;
		std::array<int, 3> outsideIndices;
		std::vector<SwrTriangle> inputTriangleBuffer;
		std::vector<SwrTriangle> bufferA;
		std::vector<SwrTriangle> bufferB;
		std::array<FrustumParam, 6> frustumParams;

		int insidePointsCount;
		int outsidePointsCount;

	public:

		Clipper()
			:
			insidePointsCount(0),
			outsidePointsCount(0)
		{
			bufferA.reserve(512);
			bufferB.reserve(512);
			frustumParams[0] = { 2, -1 };
			frustumParams[1] = { 2, 1 };
			frustumParams[2] = { 0, 1 };
			frustumParams[3] = { 0, -1 };
			frustumParams[4] = { 1, 1 };
			frustumParams[5] = { 1, -1 };
		}
		//API
		inline std::vector<SwrTriangle>& 
			ClipTriangleListWithFrustum(
				std::vector<SwrTriangle>& inputTriangles
			)
		{
			int frustumParamsSize = frustumParams.size();
			int trianglesSize = inputTriangles.size();
			/*bool isAllTrianglesWithInFrustum = true;
			for (int k = 0; k < trianglesSize; k++) {
				const auto& t = inputTriangles[k];
				if (!IsTriangleFullyInsideFrustum(t)){
					isAllTrianglesWithInFrustum = false;
					break;
				}
			}

			if (isAllTrianglesWithInFrustum){
				return inputTriangles;
			}*/

			bufferA = std::move(inputTriangles);
			for (int i = 0; i < frustumParamsSize;i++) {
				const auto& fp = frustumParams[i];
				bufferB.clear();
				int trianglesSize = bufferA.size();
				for (int j = 0; j < trianglesSize; j++) {
					const auto& t = bufferA[j];
					ClipTriangleWithFrustumPlane
						(t,
						fp.coordinateIndex,
						fp.planeSign,
						bufferB);
				}
				bufferA.swap(bufferB);
			}
			return bufferA;
		}


		
		//API
		inline bool IsVertexInsideFrustum(const SwrVertex& vertex){
			int numFrustumParams = frustumParams.size();
			float w = vertex.position.w;
			for (int i = 0; i < numFrustumParams; i++){
				const auto& fp = frustumParams[i];
				const bool isOutsideFrustum =
					(fp.planeSign < 0 &&
					vertex.position.v[fp.coordinateIndex] <-w)
					||
					(fp.planeSign > 0 &&
					vertex.position.v[fp.coordinateIndex] > w);
				if (isOutsideFrustum){
					return false;
				}
			}
			return true;
		}

		//API
		inline bool IsTriangleFullyInsideFrustum(const SwrTriangle& t){
			int numFrustumParams = frustumParams.size();
			for (int i = 0; i < 3; i++) {
				SwrVertex vertex = t.v[i];
				for (int j = 0; j < numFrustumParams;j++) {
					const auto& fp = frustumParams[j];

					const bool isOutsideFrustum =
						(fp.planeSign < 0 &&
						vertex.position.v[fp.coordinateIndex] 
						<-vertex.position.w)
						||
						(fp.planeSign > 0 &&
						vertex.position.v[fp.coordinateIndex] >vertex.position.w);
					if (isOutsideFrustum){
						return false;
					}
				}
			}
			return true;
		}
	};
}



#endif
