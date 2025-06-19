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
		inline void SetCamera(Camera *cam) {
			camera = cam;
		}

		inline void SetTexture(spn::Image* image){
			texture = image;
			textureCanvas = image->GetCanvas();
			textureCanvasWidth = textureCanvas->GetWidth();
			textureCanvasHeight = textureCanvas->GetHeight();
			textureCanvasPixels = textureCanvas->GetPixelBuffer();
			textureCanvasPitch = textureCanvas->GetPitch();
			oneMinusTextureCanvasWidth = textureCanvasWidth - 1;
			oneMinusTextureCanvasHeight = textureCanvasHeight - 1;
		}

		inline void ClampWithInCanvasWidthAndHeight(n3d::vec4f &p) {
			if (p.x >= canvasWidth){
				p.x = canvasWidth - 1;
			}
			if (p.y >= canvasHeight){
				p.y = canvasHeight - 1;
			}
		}

		inline void SetMaterial(Material* aMaterial){
			material = aMaterial;
			if (nullptr != pointLight){
				ambientColor = pointLight->ambience;
				ambientColor.r *= material->ambient.z;
				ambientColor.g *= material->ambient.y;
				ambientColor.b *= material->ambient.x;
			}
		}
		inline void SetPointLight(PointLight* aPointLight){
			pointLight = aPointLight;
			if (nullptr != material) {
				ambientColor = pointLight->ambience;
				ambientColor.r *= material->ambient.z;
				ambientColor.g *= material->ambient.y;
				ambientColor.b *= material->ambient.x;
			}
		}


		void RenderMesh(Mesh& mesh);
		void RasterizeVerticesPerspective(SwrVertex *pv0, SwrVertex *pv1, SwrVertex *pv2);
		void RasterizeVerticesOrtho(SwrVertex *pv0, SwrVertex *pv1, SwrVertex *pv2);
		void CreateDepthBuffer();
		void ClearDepthBuffer();
		void RenderDepth();
	private:
		inline void CalculateVertexColorDueToLighting(SwrVertex *vertex, MeshVertex *meshVertex) {
			color3f diffuseLight = pointLight->diffuse;
			vec3f diffuseMaterial = material->diffuse;
			vec3f lightDirHat;
			vec3f transformedNormalHat;
			color3f vertexColor = ambientColor;
			mat3x3_multiply_vec3f(&transformedNormalHat, 
				normalMatrix, 
				&meshVertex->normal);
			vec3f_norm(&transformedNormalHat, &transformedNormalHat);
			lightDirHat.x = pointLight->viewSpaceLocation.x - vertex->position.x;
			lightDirHat.y = pointLight->viewSpaceLocation.y - vertex->position.y;
			lightDirHat.z = pointLight->viewSpaceLocation.z - vertex->position.z;
			vec3f lightDirForDistance = lightDirHat;
			vec3f_norm(&lightDirHat, &lightDirHat);
			float nDotL = vec3f_dot(&lightDirHat, &transformedNormalHat);
			if (nDotL > 0){
				vertexColor.r += diffuseMaterial.z * nDotL * diffuseLight.r;
				vertexColor.g += diffuseMaterial.y * nDotL * diffuseLight.g;
				vertexColor.b += diffuseMaterial.x * nDotL * diffuseLight.b;
				float magSquared = vec3f_mag_squared(&lightDirForDistance);
				float attn = CalculateAttenuation(magSquared);
				vertexColor.b *= attn;
				vertexColor.g *= attn;
				vertexColor.r *= attn;
			}
			vertex->color = vertexColor;
		}
		inline float CalculateAttenuation(float distanceSquared) {
			const float c1 = 0.001;
			const float c2 = 0.001;
			float distance = sqrtf(distanceSquared);
			float attn = 1.0f / (1.0f + c1 * distance + c2 * distanceSquared);
			return attn;
		}
	private:
		mat4x4 matModel, matView, matViewModel, matProjection, matViewport;
		mat3x3 normalMatrix;
		Material *material;
		PointLight *pointLight;
		color3f ambientColor;

		spn::Canvas* canvas;
		float *depthBuffer;
		spn::Image* texture;
		spn::Image depthImage;
		int depthImageWidth;
		int depthImageHeight;
		unsigned char* depthImagePixels;
		unsigned int depthImagePitch;


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