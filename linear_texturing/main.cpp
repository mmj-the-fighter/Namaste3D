#include <iostream>
#include <functional>
#include "../spinach/core/spn_canvas.h"
#include "../spinach/core/spn_core.h"
#include "n3d/rasterizer.h"
#include "demo/linear_texturing.h"
#include "../spinach/common/spn_profiler.hpp"

int main(int argc, char* argv[])
{
	demo::LinearTexturing ltr;
	spn::SpinachCore sc(640, 480);
	if (sc.IsInitFailed()) {
		std::cout << "initialization failed with error "
			<< sc.GetInitializationResult()
			<< std::endl;
		return 1;
	}
	ltr.Init(&sc);
	sc.SetUpdateAndRenderHandler(
		std::bind(&demo::LinearTexturing::UpdateAndRender,
		&ltr, std::placeholders::_1));
	sc.SetInputHandler(
		std::bind(&demo::LinearTexturing::HandleInput,
		&ltr, std::placeholders::_1));
	sc.MainLoop();
	return 0;
}
