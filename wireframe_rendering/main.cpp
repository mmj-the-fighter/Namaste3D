#include <iostream>
#include <functional>
#include "../spinach/core/spn_canvas.h"
#include "../spinach/core/spn_core.h"
#include "n3d/rasterizer.h"
#include "demo/wireframe_renderer.h"

int main(int argc, char* argv[])
{
	demo::WireframeRenderer wr;
	spn::SpinachCore sc(640, 480);
	if (sc.IsInitFailed()) {
		std::cout << "initialization failed with error "
			<< sc.GetInitializationResult()
			<< std::endl;
		return 1;
	}
	wr.Init(&sc);
	sc.SetUpdateAndRenderHandler(
		std::bind(&demo::WireframeRenderer::UpdateAndRender,
		&wr, std::placeholders::_1));
	sc.SetInputHandler(
		std::bind(&demo::WireframeRenderer::HandleInput,
		&wr, std::placeholders::_1));
	sc.MainLoop();
	return 0;
}
