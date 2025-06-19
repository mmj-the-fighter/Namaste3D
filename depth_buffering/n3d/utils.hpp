#ifndef _N3D_UTILS_HPP_
#define _N3D_UTILS_HPP_

#include "vector.hpp"

namespace n3d
{
	inline void InitRandom() {
		srand(1234);
	}

	inline float MinOfThree(float a, float b, float c) {
		float minVal = a;
		if (b < minVal) {
			minVal = b;
		}
		if (c < minVal) {
			minVal = c;
		}
		return minVal;
	}

	inline float MaxOfThree(float a, float b, float c) {
		float maxVal = a;
		if (b > maxVal) {
			maxVal = b;
		}
		if (c > maxVal) {
			maxVal = c;
		}
		return maxVal;
	}

	inline float EdgeFunction(vec4f& a, vec4f& b, vec4f& c)
	{
		return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
	}
}



#endif