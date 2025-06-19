#include <iostream>
#include <functional>
#include "../spinach/core/spn_canvas.h"
#include "../spinach/core/spn_core.h"
#include "n3d/rasterizer.h"
#include "demo/points_renderer.h"

int main(int argc, char* argv[])
{
	demo::PointsRenderer pr;
	spn::SpinachCore sc(640, 480);
	if (sc.IsInitFailed()) {
		std::cout << "initialization failed with error "
			<< sc.GetInitializationResult()
			<< std::endl;
		return 1;
	}
	pr.Init(&sc);
	sc.SetUpdateAndRenderHandler(
		std::bind(&demo::PointsRenderer::UpdateAndRender,
		&pr, std::placeholders::_1));
	sc.SetInputHandler(
		std::bind(&demo::PointsRenderer::HandleInput, 
		&pr, std::placeholders::_1));
	sc.MainLoop();
	return 0;
}
