#include <iostream>
#include <functional>
#include "../spinach/core/spn_canvas.h"
#include "../spinach/core/spn_core.h"
#include "n3d/rasterizer.h"
#include "demo/scene_parsing.h"
#include "../spinach/common/spn_profiler.hpp"
#include "n3d/vector.hpp"

int main(int argc, char* argv[])
{
	demo::SceneParsing sp;
	spn::SpinachCore sc(640, 480);
	if (sc.IsInitFailed()) {
		std::cout << "initialization failed with error "
			<< sc.GetInitializationResult()
			<< std::endl;
		return 1;
	}
	sp.Init(&sc);
	sc.SetUpdateAndRenderHandler(
		std::bind(&demo::SceneParsing::UpdateAndRender,
		&sp, std::placeholders::_1));
	sc.SetInputHandler(
		std::bind(&demo::SceneParsing::HandleInput, 
		&sp, std::placeholders::_1));
	sc.MainLoop();
	sp.Destroy();
	return 0;
}
