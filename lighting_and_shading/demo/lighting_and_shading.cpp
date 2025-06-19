#include "lighting_and_shading.h"

namespace demo {
	void LightingAndShading::UpdateAndRender(spn::Canvas* canvas) {
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
				camera.SetPerspectiveCameraParams(fovy, aspectRatio, 1, 2000);
			}
		}
		if (requestedForChangeOfModel){
			requestedForChangeOfModel = false;
			LoadModel();
		}
		canvas->Clear();
		rasterizer.ClearDepthBuffer();
		n3d::mat4x4_set_roation_y(mMat, angle);
		angle += angleIncr;
		if (angle > 359.9) {
			angle = 0.0f;
		}
		rasterizer.SetModelMatrix(mMat); 
		rasterizer.SetTexture(&crateTexture);
		rasterizer.RenderMesh(mesh);

		char buffer[256];
		sprintf(buffer, "%0.2f ms", canvas->GetLastFrameTime()*1000);
		//canvas->DrawText(buffer, 245, 40);

		spinachCore->SetWindowTitle(buffer);
	}

	void LightingAndShading::HandleInput(const SDL_Event* sdlEvent) {
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
			case SDLK_PAGEUP:
				modelIndex--;
				requestedForChangeOfModel = true;
				break;
			case SDLK_PAGEDOWN:
				modelIndex++;
				requestedForChangeOfModel = true;
				break;
			case SDLK_F1:
				renderDepth = !renderDepth;
				break;
			}
		}
	}

	void LightingAndShading::LoadModel() {
		const char* modelpaths[2] = { 
			"../res/cube.obj", 
			"../res/suzanne.obj" 
		};
		int n = (sizeof(modelpaths) / sizeof(char*));
		if (modelIndex < 0){
			modelIndex = n-1;
		}
		else {
			modelIndex = modelIndex % n;
		}
		mesh.LoadObjectFile(modelpaths[modelIndex]);
	}

	void LightingAndShading::Init(spn::SpinachCore* sc){
		spinachCore = sc;
		spn::Canvas *canvas = sc->GetCanvas();
		angle = 0;
		angleIncr = 0.5;
		aspectRatio = canvas->GetAspectRatio();
		canvasWidth = canvas->GetWidth();
		canvasHeight = canvas->GetHeight();

		
		modelIndex = 0;
		LoadModel();
		debugMesh.LoadObjectFile("../res/cube.obj"),
		requestedForChangeOfModel = false;


		sc->SetWindowTitle("Spinach Demo");
		sc->SetTargetFramesPerSecond(30);
		
		canvas->SetClearColor(0, 0, 0);
		canvas->SetPrimaryColor(192, 192, 0);
		rasterizer.SetCanvas(canvas);

		eye.x = 0;
		eye.y = 0;
		eye.z = 10;

		at.x = 0;
		at.y = 0;
		at.z = 0;

		up.x = 0;
		up.y = 1;
		up.z = 0;

		camera.SetViewParams(eye, at, up);
		fovy = 45;
		camera.SetPerspectiveCameraParams(fovy, aspectRatio, 1, 2000);

		halfHorizontalExtend = 2 * aspectRatio;
		halfVerticalExtend = 2;



		rasterizer.SetCamera(&camera);

		n3d::mat4x4_set_viewport(viewportMat, 0, 0, canvasWidth, canvasHeight, 1);
		rasterizer.SetViewportMatrix(viewportMat);

		if (crateTexture.CreateFromPng("../res/crate.png") == false){
			std::cout << "loading texture failed\n";
		}
		rasterizer.SetTexture(&crateTexture);
		rasterizer.CreateDepthBuffer();

		requestedForChangeOfCamera = false;
		renderDepth = false;

		pointLight.ambience.b = 20;
		pointLight.ambience.g = 20;
		pointLight.ambience.r = 20;

		pointLight.diffuse.b = 192;
		pointLight.diffuse.g = 32;
		pointLight.diffuse.r = 0;

		pointLight.worldSpaceLocation.x = 0;
		pointLight.worldSpaceLocation.y = 0;
		pointLight.worldSpaceLocation.z = 10;
		pointLight.worldSpaceLocation.w = 1;

		material.ambient.x = 0.4;
		material.ambient.y = 0.2;
		material.ambient.z = 0.1;

		material.diffuse.x = 0.4;
		material.diffuse.y = 0.2;
		material.diffuse.z = 0.1;

		rasterizer.SetPointLight(&pointLight);
		rasterizer.SetMaterial(&material);
	}
}