#include <iostream>
#include <functional>
#include "../spinach/core/spn_canvas.h"
#include "../spinach/core/spn_core.h"
#include "n3d/rasterizer.h"
#include "demo/depth_buffering.h"

int main(int argc, char* argv[])
{
	demo::DepthBuffering dbr;
	spn::SpinachCore sc(640, 480);
	if (sc.IsInitFailed()) {
		std::cout << "initialization failed with error "
			<< sc.GetInitializationResult()
			<< std::endl;
		return 1;
	}
	dbr.Init(&sc);
	sc.SetUpdateAndRenderHandler(
		std::bind(&demo::DepthBuffering::UpdateAndRender,
		&dbr, std::placeholders::_1));
	sc.SetInputHandler(
		std::bind(&demo::DepthBuffering::HandleInput,
		&dbr, std::placeholders::_1));
	sc.MainLoop();
	return 0;
}
