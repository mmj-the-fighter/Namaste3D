#include <iostream>
#include "rasterizer.h"
#include "clipper.hpp"

#define SPLITTRIANGLESWHILECLIPPING

namespace n3d
{
	Rasterizer::Rasterizer() :canvas(nullptr){
		inputTriList.reserve(10);
		clippedTriList.reserve(512);
	}

	Rasterizer::~Rasterizer(){
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

			SwrVertex vertexB;
			vec4f_make_point_from_vec3f(&vertexB.position, &mesh.vertices[i+1].position);

			SwrVertex vertexC;
			vec4f_make_point_from_vec3f(&vertexC.position, &mesh.vertices[i + 2].position);

			
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
				vec4f pointA = tri.a.position;
				vec4f pointB = tri.b.position;
				vec4f pointC = tri.c.position;

				float s = 1 / pointA.w;
				pointA.x *= s;
				pointA.y *= s;
				pointA.z *= s;
				pointA.w = 1.0;
				mat4x4_transform(&pointA, matViewport);
				

				s = 1 / pointB.w;
				pointB.x *= s;
				pointB.y *= s;
				pointB.z *= s;
				pointB.w = 1.0;
				mat4x4_transform(&pointB, matViewport);
				
				s = 1 / pointC.w;
				pointC.x *= s;
				pointC.y *= s;
				pointC.z *= s;
				pointC.w = 1.0;
				mat4x4_transform(&pointC, matViewport);
				
				ClampWithInCanvasWidthAndHeight(pointA);
				ClampWithInCanvasWidthAndHeight(pointB);
				ClampWithInCanvasWidthAndHeight(pointC);

				float twoA = compute_two_times_triangle_area_vec4f(&pointA, &pointB, &pointC);
				if (twoA >= 0) {
					continue;
				}

				canvas->DrawLine(pointA.x, pointA.y, pointB.x, pointB.y);
				canvas->DrawLine(pointB.x, pointB.y, pointC.x, pointC.y);
				canvas->DrawLine(pointC.x, pointC.y, pointA.x, pointA.y);
			}
		}
	}
}