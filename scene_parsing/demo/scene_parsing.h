#ifndef _POINTS_RENDERER_H_
#define _POINTS_RENDERER_H_

#include "../../spinach/core/spn_canvas.h"
#include "../../spinach/core/spn_core.h"
#include "../../spinach/core/spn_image.h"
#include "../../spinach/common/spn_profiler.hpp"
#include "../n3d/n3d.h"
#include "../n3d/rasterizer.h"
#include "../n3d/scene.h"
#include "../n3d/resource_loader.h"


namespace demo{
	class SceneParsing
	{
	public:
		~SceneParsing() {
			delete scene;
		}
		void Init(spn::SpinachCore* core);
		void UpdateAndRender(spn::Canvas* canvas);
		void HandleInput(const SDL_Event* sdlEvent);
		void Destroy();
	private:
		
		n3d::Scene* scene;
		n3d::Rasterizer  rasterizer;
		n3d::Camera camera;
		
		spn::SpinachCore* spinachCore;
		n3d::ProjectionPart pp;
		n3d::ViewPart vp;
		n3d::PointLightPart plp;
		float aspectRatio;
		float canvasWidth;
		float canvasHeight;
		float halfHorizontalExtend;
		float halfVerticalExtend;
		bool requestedForChangeOfCamera;
		n3d::PointLight pointLight;
		n3d::Material material;
		n3d::ResourceLoader resourceLoader;
	};
}

#endif;