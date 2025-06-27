#ifndef _N3D_PLANE_HPP_
#define _N3D_PLANE_HPP_

#include "vector.hpp"
#define EPSILON 0.000001f

namespace n3d {


	enum PointWrtoPlane {
		FRONT_OF_PLANE,
		BACK_OF_PLANE,
		ON_PLANE
	};

	typedef struct _plane{
		float A;
		float B;
		float C;
		float D;
	}plane;

	inline int is_plane_valid(plane pln) {
		return (pln.A != 0 || pln.B != 0 || pln.C != 0);
	}
	inline int is_plane_invalid(plane pln) {
		return (pln.A == 0 && pln.B == 0 && pln.C == 0);
	}

	inline int plane_init_three_points(plane* pln, vec3f* p1, vec3f* p2, vec3f* p3) {
		// Create vectors from points
		float v1x = p2->x - p1->x;
		float v1y = p2->y - p1->y;
		float v1z = p2->z - p1->z;

		float v2x = p3->x - p1->x;
		float v2y = p3->y - p1->y;
		float v2z = p3->z - p1->z;

		// Calculate the normal vector (A, B, C) using the cross product
		float A = v1y * v2z - v1z * v2y;
		float B = v1z * v2x - v1x * v2z;
		float C = v1x * v2y - v1y * v2x;


		// Check for collinearity (normal vector should not be zero)
		if (A == 0 && B == 0 && C == 0) {
			return 0;
		}


		// Normalize the normal vector
		float  oneOverLength = 1.0/std::sqrt(A * A + B * B + C * C);
		A *= oneOverLength;
		B *= oneOverLength;
		C *= oneOverLength;

		pln->A = A;
		pln->B = B;
		pln->C = C;

		// Calculate D
		pln->D = -(A * p1->x + B * p1->y + C * p1->z);
		return 1;
	}

	inline void plane_normalize(plane* result, plane* pl) {
		float oneOverlength = 1.0 / sqrt(pl->A * pl->A + pl->B * pl->B + pl->C * pl->C);
		result->A *= oneOverlength;
		result->B *= oneOverlength;
		result->C *= oneOverlength;
		result->D *= oneOverlength;
	}

	
	inline float plane_signed_distance(plane* pl, vec3f* pt) {
		float sd = pl->A * pt->x + pl->B * pt->y + pl->C * pt->z + pl->D;
		return sd;
	}

	inline PointWrtoPlane plane_classify_point(plane* pl, vec3f* pt) {
		float sd = pl->A * pt->x + pl->B * pt->y + pl->C * pt->z + pl->D;
		if (sd > EPSILON) {
			return FRONT_OF_PLANE;
		}
		else if (sd < -EPSILON) {
			return BACK_OF_PLANE;
		}
		else {
			return ON_PLANE;
		}
	}

}

#endif

