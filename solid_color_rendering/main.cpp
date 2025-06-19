#include <iostream>
#include <functional>
#include "../spinach/core/spn_canvas.h"
#include "../spinach/core/spn_core.h"
#include "n3d/rasterizer.h"
#include "demo/solid_color_renderer.h"

int main(int argc, char* argv[])
{
	demo::SolidColorRenderer sr;
	spn::SpinachCore sc(640, 480);
	if (sc.IsInitFailed()) {
		std::cout << "initialization failed with error "
			<< sc.GetInitializationResult()
			<< std::endl;
		return 1;
	}
	sr.Init(&sc);
	sc.SetUpdateAndRenderHandler(
		std::bind(&demo::SolidColorRenderer::UpdateAndRender,
		&sr, std::placeholders::_1));
	sc.SetInputHandler(
		std::bind(&demo::SolidColorRenderer::HandleInput,
		&sr, std::placeholders::_1));
	sc.MainLoop();
	return 0;
}
