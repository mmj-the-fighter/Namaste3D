#include <iostream>
#include <functional>
#include "../spinach/core/spn_canvas.h"
#include "../spinach/core/spn_core.h"
#include "n3d/rasterizer.h"
#include "demo/lighting_and_shading.h"

int main(int argc, char* argv[])
{
	demo::LightingAndShading landsh;
	spn::SpinachCore sc(640, 480);
	if (sc.IsInitFailed()) {
		std::cout << "initialization failed with error "
			<< sc.GetInitializationResult()
			<< std::endl;
		return 1;
	}
	landsh.Init(&sc);
	sc.SetUpdateAndRenderHandler(
		std::bind(&demo::LightingAndShading::UpdateAndRender,
		&landsh, std::placeholders::_1));
	sc.SetInputHandler(
		std::bind(&demo::LightingAndShading::HandleInput,
		&landsh, std::placeholders::_1));
	sc.MainLoop();
	return 0;
}
