#ifndef _N3D_RASTERIZER_H_
#define _N3D_RASTERIZER_H_

#include <cstring>

#include "../../spinach/core/spn_canvas.h"
#include "mesh.h"
#include "vector.hpp"
#include "matrix.hpp"
#include "n3d.h"
#include "clipper.hpp"
#include "camera.hpp"


namespace n3d {
	class Rasterizer{
	public:
		Rasterizer();
		~Rasterizer();
		inline void SetCanvas(spn::Canvas* aCanvas){
			canvasWidth = aCanvas->GetWidth();
			canvasHeight = aCanvas->GetHeight();
			canvas = aCanvas;
		}
		inline void SetModelMatrix(mat4x4 m){
			memcpy(matModel, m, 16 * sizeof(float));
		}
		inline void SetViewMatrix(mat4x4 m) {
			memcpy(matView, m, 16 * sizeof(float));
		}
		void SetProjectionMatrix(mat4x4 m) {
			memcpy(matProjection, m, 16 * sizeof(float));
		}
		void SetViewportMatrix(mat4x4 m) {
			memcpy(matViewport, m, 16 * sizeof(float));
		}
		void SetCamera(Camera *cam) {
			camera = cam;
		}
		void RenderMesh(Mesh& mesh);
	private:
		mat4x4 matModel, matView, matProjection, matViewport;
		spn::Canvas* canvas;
		Clipper clipper;
		Camera* camera;
		int canvasWidth;
		int canvasHeight;
	};
}

#endif