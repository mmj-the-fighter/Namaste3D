#include <iostream>
#include <cstring>
#include "../common/spn_utils.h"
#include "spn_core.h"


//#define SHOWFRAMESTATS
#define SINGLEMEMCOPY
#define DEFAULTFPS 64


namespace spn
{
	void SetUpdateAndRenderHandler(std::function<void(Canvas* canvas)>);
	void SetInputHandler(std::function<void(const SDL_Event* sdlEvent)>);

	SpinachCore::SpinachCore(unsigned int width, unsigned int height, 
		std::function<void(Canvas* canvas)> updateAndRenderFn, 
		std::function<void(const SDL_Event* sdlEvent)> inputFn
		) :
		window(nullptr),
		renderer(nullptr),
		texture(nullptr),
		canvas(nullptr)
	{
		SetTargetFramesPerSecond(DEFAULTFPS);
		SetUpdateAndRenderHandler(updateAndRenderFn);
		SetInputHandler(inputFn);
		initializationResult = Init(width, height);
	}

	SpinachCore::~SpinachCore()
	{
		Destroy();
	}

	void SpinachCore::SetTargetFramesPerSecond(unsigned int aFps)
	{
		targetFramesPerSecond = aFps;
		targetMillisPerFrame = 1000.0f / static_cast<float>(aFps);
	}


	int SpinachCore::Init(unsigned int width, unsigned int height)
	{
		userWantsToQuit = 0;

		if (!SDL_Init(SDL_INIT_VIDEO))
		{
			std::cout << "Couldn't initialize SDL:" << SDL_GetError() << std::endl;
			return 1;
		}

		if (!SDL_CreateWindowAndRenderer(
			"Spinach App",
			width,
			height,
			0,
			&window,
			&renderer))
		{
			std::cout << "Couldn't create window or renderer:" << SDL_GetError() << std::endl;
			return 2;
		}

		texture = SDL_CreateTexture(renderer,
			SDL_PIXELFORMAT_ARGB8888,
			SDL_TEXTUREACCESS_STREAMING,
			width,
			height);
		if (!texture)
		{
			std::cout << "Couldn't create streaming texture: " << SDL_GetError() << std::endl;
			return 3;
		}
		std::string atlasName{ "../res/TrueNoFontAtlas.ppm" };
		std::string csvName{ "../res/TrueNoFontData.csv" };
		font = new RFont(atlasName, csvName);
		if (!font->IsInitSucceded()){
			return 4;
		}
		canvas = new Canvas(width, height);
		canvas->SetFont(font);
		canvas->Clear();
		canvas->DrawText("Spinach Initialized..", 0, 0);
		RenderCanvas();

		return 0;
	}

	void SpinachCore::SetUpdateAndRenderHandler(std::function<void(Canvas*)> aUpdateAndRenderHandler)
	{
		updateAndRenderHandler = aUpdateAndRenderHandler;
	}

	void SpinachCore::SetInputHandler(std::function<void(const SDL_Event* sdlEvent)> aInputHandler){
		inputHandler = aInputHandler;
	}

	void SpinachCore::MainLoop()
	{
		SDL_Event event;
		SDL_zero(event);
		Uint32 frameStartTime, frameProcTime = 0, waitTime = 0;
#ifdef SHOWFRAMESTATS
		Uint32 minFrameProcTime = 1000000;
		Uint32 maxFrameProcTime = 0;
#endif
		int height = canvas->GetHeight();
		int pitch = canvas->GetPitch();
		int bufferBytesLength = height * pitch;
		unsigned char* pixels = canvas->GetPixelBuffer();
		userWantsToQuit = 0;
		while (0 == userWantsToQuit)
		{
			frameStartTime = SDL_GetTicks();
			//canvas->SetLastFrameTime(static_cast<float>(frameProcTime + waitTime) / 1000.0f);
			canvas->SetLastFrameTime(static_cast<float>(frameProcTime) / 1000.0f);
			while (SDL_PollEvent(&event))
			{
				switch (event.type){
				case SDL_EVENT_QUIT:
					userWantsToQuit = 1;
#ifdef SHOWFRAMESTATS
					std::cout << "min: " << minFrameProcTime << "\tcur:" << frameProcTime << "\tmax:" << maxFrameProcTime << std::endl;
#endif
					break;
				case SDL_EVENT_KEY_DOWN:
					switch (event.key.key)
					{
					case SDLK_ESCAPE:
						userWantsToQuit = 1;
						break;
					case SDLK_F12:
					{
						std::string fileName = GetTimeBasedScreenShotFileName();
						SaveScreenShot(fileName);
					}
						break;
					}
				default:
					if (nullptr != inputHandler){
						inputHandler(&event);
					}
					break;
				}
			}

			if (nullptr != updateAndRenderHandler) {
				updateAndRenderHandler(canvas);
			}

			unsigned char* destPixels;
			int destPitch;
			if (SDL_LockTexture(texture, NULL, (void**)&destPixels, &destPitch))
			{
#ifdef SINGLEMEMCOPY
				memcpy(destPixels, pixels, bufferBytesLength);
#else
				for (int y = 0; y < height; ++y) {
					memcpy(destPixels + y * destPitch, pixels + y * pitch, pitch);
				}
#endif
				SDL_UnlockTexture(texture);
			}
			SDL_RenderTexture(renderer, texture, NULL, NULL);
			SDL_RenderPresent(renderer);
			frameProcTime = (SDL_GetTicks() - frameStartTime);
#ifdef SHOWFRAMESTATS
			if (frameProcTime < minFrameProcTime){
				minFrameProcTime = frameProcTime;
			}
			else if (frameProcTime > maxFrameProcTime){
				maxFrameProcTime = frameProcTime;
			}
			std::cout << frameProcTime << "\n";
#endif

			if (frameProcTime < targetMillisPerFrame) {
				waitTime = targetMillisPerFrame - frameProcTime;
				SDL_Delay(waitTime);
			}
			else{
				waitTime = 0;
			}
		}
	}

	void SpinachCore::SaveScreenShot(const std::string& fileName)
	{
		image.SetCanvas(canvas);
		image.SaveAsPng(fileName);
		image.SetCanvas(nullptr);
	}

	void SpinachCore::WaitForEvents()
	{
		SDL_Event event;
		int loop = 1;
		while (loop) {
			SDL_zero(event);
			SDL_PollEvent(&event);
			if (event.type == SDL_EVENT_KEY_UP ||
				event.type == SDL_EVENT_QUIT ||
				event.type == SDL_EVENT_MOUSE_BUTTON_UP) {
				loop = 0;
			}

		}
	}

	void SpinachCore::RenderCanvas()
	{
		int height = canvas->GetHeight();
		int pitch = canvas->GetPitch();
		int bufferBytesLength = height * pitch;
		unsigned char* pixels = canvas->GetPixelBuffer();
		unsigned char* destPixels;
		int destPitch;
		if (SDL_LockTexture(texture, NULL, (void**)&destPixels, &destPitch))
		{
#ifdef SINGLEMEMCOPY
			memcpy(destPixels, pixels, bufferBytesLength);
#else
			for (int y = 0; y < height; ++y) {
				memcpy(destPixels + y * destPitch, pixels + y * pitch, pitch);
			}
#endif
			SDL_UnlockTexture(texture);
		}
		SDL_RenderTexture(renderer, texture, NULL, NULL);
		SDL_RenderPresent(renderer);
	}

	void SpinachCore::Destroy()
	{
		if (texture != nullptr) {
			SDL_DestroyTexture(texture);
		}

		if (canvas != nullptr) {
			delete canvas;
		}
	}
}
