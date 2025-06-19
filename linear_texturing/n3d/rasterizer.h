#ifndef _N3D_RASTERIZER_H_
#define _N3D_RASTERIZER_H_

#include <cstring>

#include "../../spinach/core/spn_canvas.h"
#include "../../spinach/core/spn_image.h"
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
			canvasPixels = aCanvas->GetPixelBuffer();
			canvasPitch = aCanvas->GetPitch();
			canvas = aCanvas;
		}
		inline void SetModelMatrix(mat4x4 m){
			memcpy(matModel, m, 16 * sizeof(float));
		}
		inline void SetViewMatrix(mat4x4 m) {
			memcpy(matView, m, 16 * sizeof(float));
		}
		inline void SetProjectionMatrix(mat4x4 m) {
			memcpy(matProjection, m, 16 * sizeof(float));
		}
		inline void SetViewportMatrix(mat4x4 m) {
			memcpy(matViewport, m, 16 * sizeof(float));
		}
		void SetCamera(Camera *cam) {
			camera = cam;
		}

		void SetTexture(spn::Image* image){
			texture = image;
			textureCanvas = image->GetCanvas();
			textureCanvasWidth = textureCanvas->GetWidth();
			textureCanvasHeight = textureCanvas->GetHeight();
			textureCanvasPixels = textureCanvas->GetPixelBuffer();
			textureCanvasPitch = textureCanvas->GetPitch();
			oneMinusTextureCanvasWidth = textureCanvasWidth - 1;
			oneMinusTextureCanvasHeight = textureCanvasHeight - 1;
		}

		void ClampWithInCanvasWidthAndHeight(n3d::vec4f &p) {
			if (p.x >= canvasWidth){
				p.x = canvasWidth - 1;
			}
			if (p.y >= canvasHeight){
				p.y = canvasHeight - 1;
			}
		}


		void RenderMesh(Mesh& mesh);
		void RasterizeVertices(SwrVertex *pv0, SwrVertex *pv1, SwrVertex *pv2);
	private:
		mat4x4 matModel, matView, matProjection, matViewport;
		spn::Canvas* canvas;
		spn::Image* texture;
		spn::Canvas* textureCanvas;
		Clipper clipper;
		Camera* camera;
		int canvasWidth;
		int canvasHeight;
		unsigned char* canvasPixels;
		unsigned int canvasPitch;

		int textureCanvasWidth;
		int textureCanvasHeight;
		int oneMinusTextureCanvasWidth;
		int oneMinusTextureCanvasHeight;
		unsigned char* textureCanvasPixels;
		unsigned int textureCanvasPitch;
		std::vector<SwrTriangle> inputTriList;
		std::vector<SwrTriangle> clippedTriList;
	};
}

#endif