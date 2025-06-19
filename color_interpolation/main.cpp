#include <iostream>
#include <functional>
#include "../spinach/core/spn_canvas.h"
#include "../spinach/core/spn_core.h"
#include "n3d/rasterizer.h"
#include "demo/color_interpolation.h"

int main(int argc, char* argv[])
{
	demo::ColorInterpolation ci;
	spn::SpinachCore sc(640, 480);
	if (sc.IsInitFailed()) {
		std::cout << "initialization failed with error "
			<< sc.GetInitializationResult()
			<< std::endl;
		return 1;
	}
	ci.Init(&sc);
	sc.SetUpdateAndRenderHandler(
		std::bind(&demo::ColorInterpolation::UpdateAndRender,
		&ci, std::placeholders::_1));
	sc.SetInputHandler(
		std::bind(&demo::ColorInterpolation::HandleInput,
		&ci, std::placeholders::_1));
	sc.MainLoop();
	return 0;
}
