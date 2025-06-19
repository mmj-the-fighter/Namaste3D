#ifndef _IMAGE_H_
#define _IMAGE_H_

#include <string>

namespace spn
{
	class Canvas;

	class Image
	{
	public:
		Image(unsigned int aWidth, unsigned int aHeight);
		Image(Canvas* aCanvas);
		Image();
		~Image();
		void CreateSolidColorBlockImage(int width, int height,
			int colorR, int colorG, int colorB, float alpha);
		void CreateCheckerImage(int width, int height, 
			int tileSize, 
			int color1R, int color1G, int color1B, 
			int color2R, int color2G, int color2B);
		bool CreateFromPng(const std::string& fileName);
		bool CreateFromPpmRaw(const std::string& fileName);
		bool CreateFromPpmAscii(const std::string& fileName);
		bool SaveAsPng(const std::string& fileName);
		inline Canvas* GetCanvas() {
			return canvas;
		}
		inline void SetCanvas(Canvas *aCanvas) {
			canvas = aCanvas;
		}
		Image* Clone();

	private:
		Canvas *canvas;
		void DestroyCanvas();
	};
}

#endif 
