#ifndef _N3D_CLIPPER_HPP_
#define _N3D_CLIPPER_HPP_
#include <vector>
#include <array>
#include <iostream>
#include "vector.hpp"
#include "rasterizer.h"

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
		std::array<FrustumParam, 6> frustumParams;
	public:
		Clipper()
		{
			frustumParams[0] = { 2, -1 };
			frustumParams[1] = { 2, 1 };
			frustumParams[2] = { 0, 1 };
			frustumParams[3] = { 0, -1 };
			frustumParams[4] = { 1, 1 };
			frustumParams[5] = { 1, -1 };
		}

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
	};
}



#endif
