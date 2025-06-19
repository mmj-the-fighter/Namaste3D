#ifndef _POINTS_RENDERER_H_
#define _POINTS_RENDERER_H_

#include "../../spinach/core/spn_canvas.h"
#include "../../spinach/core/spn_core.h"
#include "../n3d/rasterizer.h"

namespace demo{
	class PointsRenderer
	{
	public:
		void Init(spn::SpinachCore* core);
		void UpdateAndRender(spn::Canvas* canvas);
		void HandleInput(const SDL_Event* sdlEvent);
		void LoadModel();
	private:
		n3d::Rasterizer  rasterizer;
		spn::SpinachCore* spinachCore;
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