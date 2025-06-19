#include <iostream>
#include <functional>
#include "../spinach/core/spn_canvas.h"
#include "../spinach/core/spn_core.h"
#include "n3d/rasterizer.h"
#include "demo/perspective_correct_texturing.h"

int main(int argc, char* argv[])
{
	demo::PerspectiveCorrectTexturing pctr;
	spn::SpinachCore sc(640, 480);
	if (sc.IsInitFailed()) {
		std::cout << "initialization failed with error "
			<< sc.GetInitializationResult()
			<< std::endl;
		return 1;
	}
	pctr.Init(&sc);
	sc.SetUpdateAndRenderHandler(
		std::bind(&demo::PerspectiveCorrectTexturing::UpdateAndRender,
		&pctr, std::placeholders::_1));
	sc.SetInputHandler(
		std::bind(&demo::PerspectiveCorrectTexturing::HandleInput,
		&pctr, std::placeholders::_1));
	sc.MainLoop();
	return 0;
}
