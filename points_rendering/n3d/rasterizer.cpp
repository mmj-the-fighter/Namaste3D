#include <iostream>
#include "rasterizer.h"
#include "clipper.hpp"

namespace n3d
{
	Rasterizer::Rasterizer() :canvas(nullptr){}

	Rasterizer::~Rasterizer(){
	}

	void Rasterizer::RenderMesh(Mesh& mesh) {
		camera->Sync(matView, matProjection);
		mat4x4 vm;
		mat4x4_multiply(vm, matView, matModel);
		mat4x4 pvm;
		mat4x4_multiply(pvm, matProjection, vm);

		int i;
		int n = mesh.vertices.size();
		for (i = 0; i < n; ++i) {
			SwrVertex vertex;
			vec4f_make_point_from_vec3f(&vertex.position, &mesh.vertices[i].position);
			mat4x4_transform(&vertex.position, pvm);
			if (clipper.IsVertexInsideFrustum(vertex)) {
				vec4f point = vertex.position;
				float s = 1 / point.w;
				point.x *= s;
				point.y *= s;
				point.z *= s;
				point.w = 1.0;
				mat4x4_transform(&point, matViewport);

				int pixelX = static_cast<int>(point.x + 0.5);
				int pixelY = static_cast<int>(point.y + 0.5);

				if (pixelX >= canvasWidth) {
					pixelX = canvasWidth - 1;
				}

				if (pixelY >= canvasHeight) {
					pixelY = canvasHeight-1;
				}

				canvas->SetPixel(pixelX, pixelY);
			}
		}
	}
}