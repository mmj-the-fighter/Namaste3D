#include "scene_parsing.h"


namespace demo {


	void SceneParsing::UpdateAndRender(spn::Canvas* canvas) {
		int cullCount = 0;
		if (requestedForChangeOfCamera) {
			requestedForChangeOfCamera = false;
			if (camera.IsPerspective()) {
				camera.SetOrthoCameraParams(
					-halfHorizontalExtend,
					halfHorizontalExtend,
					-halfVerticalExtend,
					halfVerticalExtend,
					-20.0,
					20.0);
			}
			else {
				camera.SetPerspectiveCameraParams(pp.fovy, aspectRatio, pp.near, pp.far);
			}
		}
		canvas->Clear();
		rasterizer.ClearDepthBuffer();
		
		scene->GeometryStart();
		while (!scene->IsGeometryEnded()){
			n3d::GeometricObjectPart geom;
			scene->GetGeometryAndIterate(geom);
			rasterizer.SetModelMatrix(geom.modelMat);
			rasterizer.SetMaterial(geom.material);
			rasterizer.SetTexture(geom.texture);
			bool isRendered = rasterizer.RenderMesh(*geom.mesh);
			if (!isRendered){
				++cullCount;
			}
		}
		char buffer[256];
		sprintf(buffer, "%0.2f ms culled %d", canvas->GetLastFrameTime() * 1000, cullCount);
		canvas->DrawText(buffer, 245, 40);
	}


	void SceneParsing::Destroy(){
		std::cout << "Initialization duration: ";
		spn::Profiler::GetInstance().Print();
	}

	void SceneParsing::HandleInput(const SDL_Event* sdlEvent) {
		if (sdlEvent->type == SDL_EVENT_KEY_DOWN){
			switch (sdlEvent->key.key)
			{
			case SDLK_J:
				camera.MoveCamera(n3d::DOWN);
				break;
			case SDLK_U:
				camera.MoveCamera(n3d::UP);
				break;

			case SDLK_A:
				camera.TurnLeft();
				break;

			case SDLK_D:
				camera.TurnRight();
				break;

			case SDLK_W:
				camera.LookDown();
				break;

			case SDLK_S:
				camera.LookUp();
				break;

			case SDLK_LEFT:
				camera.MoveCamera(n3d::LEFT);
				break;
			case SDLK_RIGHT:
				camera.MoveCamera(n3d::RIGHT);
				break;
			case SDLK_UP:
				camera.MoveCamera(n3d::FORWARD);
				break;
			case SDLK_DOWN:
				camera.MoveCamera(n3d::BACKWARD);
				break;
			case SDLK_O:
				if (camera.IsPerspective()) {
					requestedForChangeOfCamera = true;
				}
				break;
			case SDLK_P:
				if (!camera.IsPerspective()){
					requestedForChangeOfCamera = true;
				}
				break;
			case SDLK_R:
				camera.SetViewParams(vp.eye, vp.at, vp.up);
				camera.SetCameraViewNeedsUpdate(true);
				break;
			}
		}
	}


	void SceneParsing::Init(spn::SpinachCore* sc){
		spn::Profiler::GetInstance().Begin(123);
		//SDL_Delay(1200);
		spinachCore = sc;
		spn::Canvas *canvas = sc->GetCanvas();
		aspectRatio = canvas->GetAspectRatio();
		canvasWidth = canvas->GetWidth();
		canvasHeight = canvas->GetHeight();
		scene = new n3d::Scene(&resourceLoader);
		sc->SetWindowTitle("Spinach Demo");
		sc->SetTargetFramesPerSecond(30);
		canvas->SetClearColor(0, 0, 0);
		canvas->SetPrimaryColor(192, 192, 0);
		rasterizer.SetCanvas(canvas);
		scene->LoadSceneFile("../res/scene.txt");
		scene->GetInitializedParts(pp, vp, plp);
		camera.SetViewParams(vp.eye, vp.at, vp.up);
		camera.SetPerspectiveCameraParams(pp.fovy, aspectRatio, pp.near, pp.far);
		halfHorizontalExtend = 2 * aspectRatio;
		halfVerticalExtend = 2;
		rasterizer.SetCamera(&camera);
		n3d::mat4x4 viewportMat;
		n3d::mat4x4_set_viewport(viewportMat, 0, 0, canvasWidth, canvasHeight, 1);
		rasterizer.SetViewportMatrix(viewportMat);
		rasterizer.CreateDepthBuffer();
		requestedForChangeOfCamera = false;
		rasterizer.SetPointLight(&plp.pointLight);
		spn::Profiler::GetInstance().End();
	}
}