#ifndef _LINEAR_TEXTURING_H
#define _LINEAR_TEXTURING_H

#include "../../spinach/core/spn_canvas.h"
#include "../../spinach/core/spn_core.h"
#include "../../spinach/core/spn_image.h"
#include "../n3d/rasterizer.h"

namespace demo{
	class LinearTexturing
	{
	public:
		void Init(spn::SpinachCore* core);
		void UpdateAndRender(spn::Canvas* canvas);
		void HandleInput(const SDL_Event* sdlEvent);
		void LoadModel();
	private:
		n3d::Rasterizer  rasterizer;
		n3d::Mesh mesh;
		n3d::Camera camera;
		n3d::vec3f eye;
		n3d::vec3f at;
		n3d::vec3f up;
		n3d::mat4x4 mMat;
		n3d::mat4x4 vMat;
		n3d::mat4x4 pMat;
		n3d::mat4x4 vmMat;
		n3d::mat4x4 pvmMat;
		n3d::mat4x4 viewportMat;
		spn::SpinachCore* spinachCore;
		spn::Image checkerTexture;
		spn::Image fTexture;
		spn::Image lenaTexture;
		float angle;
		float angleIncr;
		float aspectRatio;
		float canvasWidth;
		float canvasHeight;
		float halfHorizontalExtend;
		float halfVerticalExtend;
		float fovy;
		bool requestedForChangeOfModel;
		bool requestedForChangeOfCamera;
		int modelIndex;
	};
}

#endif;