#include <fstream>
#include "spn_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../external/stb/stb_image_write.h"


#define STB_IMAGE_IMPLEMENTATION
#include "../external/stb/stb_image.h"

#include "spn_canvas.h"

namespace spn
{
	Image::Image(unsigned int aWidth, unsigned int aHeight)
	{
		canvas = new Canvas(aWidth, aHeight);
	}

	Image::Image(Canvas* aCanvas)
	{
		canvas = aCanvas;
	}

	Image::Image()
	{
		canvas = nullptr;
	}

	Image::~Image()
	{
		DestroyCanvas();
	}


	void Image::DestroyCanvas()
	{
		if (nullptr != canvas){
			delete canvas;
			canvas = nullptr;
		}
	}

	Image* Image::Clone()
	{
		Image * newImage = new Image();
		if (nullptr != canvas)
		{
			Canvas * newCanvas = canvas->Clone();
			newImage->SetCanvas(newCanvas);
		}
		return newImage;
	}

	void Image::CreateSolidColorBlockImage(int width, int height,
		int colorR, int colorG, int colorB, float alpha)
	{
		canvas = new Canvas(width, height);
		unsigned char* pixels = canvas->GetPixelBuffer();
		int pitch = canvas->GetPitch();
		int colorA = 255.0;
		for (int y = 0; y < height; ++y) {
			for (int x = 0; x < width; ++x) {
				unsigned char* loc = pixels + pitch * y + x * 4;
				*loc = colorB;
				++loc;
				*loc = colorG;
				++loc;
				*loc = colorR;
				++loc;
				*loc = colorA;

			}
		}

	}

	//AI is used for making this function
	void Image::CreateCheckerImage(int width, int height, 
		int tileSize, 
		int color1R, int color1G, int color1B, 
		int color2R, int color2G, int color2B) 
	{
		canvas = new Canvas(width, height);
		unsigned char* pixels = canvas->GetPixelBuffer();
		int pitch = canvas->GetPitch();

		for (int y = 0; y < height; ++y) {
			for (int x = 0; x < width; ++x) {
				unsigned char* loc = pixels + pitch * y + x * 4;
				bool isColor1 = (((x / tileSize) & 1) == ((y / tileSize) & 1));
				if (isColor1) {
					*loc++ = color1B;
					*loc++ = color1G;
					*loc++ = color1R;
					*loc = 255;
				}
				else {
					*loc++ = color2B;
					*loc++ = color2G;
					*loc++ = color2R;
					*loc = 255;
				}
			}
		}
	}

	bool Image::CreateFromPng(const std::string& fileName)
	{
		if (nullptr != canvas) {
			return false;
		}
		int w, h, bpp;
		int num_of_pixels;
		int i;
		unsigned char *srcloc, *dstloc;
		unsigned char* imgdata_stb = stbi_load(fileName.c_str(), &w, &h, &bpp, STBI_rgb_alpha);
		if (nullptr == imgdata_stb) {
			return false;
		}
		canvas = new Canvas(w, h);
		dstloc = canvas->GetPixelBuffer();
		srcloc = imgdata_stb;
		num_of_pixels = canvas->GetNumOfPixels();

		unsigned char rgba[4];
		for (i = 0; i < num_of_pixels; ++i) {
			rgba[0] = *srcloc++;
			rgba[1] = *srcloc++;
			rgba[2] = *srcloc++;
			rgba[3] = *srcloc++;
			*dstloc++ = rgba[2];
			*dstloc++ = rgba[1];
			*dstloc++ = rgba[0];
			*dstloc++ = rgba[3];
		}
		stbi_image_free(imgdata_stb);
		return true;
	}

	bool Image::CreateFromPpmRaw(const std::string& fileName)
	{
		unsigned char buffer[3];
		int width = 0, height = 0, colorDepth = 0;
		
		std::ifstream file(fileName, std::ios::binary);
		if (!file.is_open()) {
			return false;
		}

		char c;

		file.read((char*)buffer, 3);
		if (!file) {
			return false;
		}
		if (buffer[0] != 'P' || buffer[1] != '6') {
			return false;
		}

		c = file.peek();
		if (c == '#') {
			do{
				file.get(c);
			} while (c != '\n');
		}
		file >> width;
		file >> height;
		file >> colorDepth;

		file.get();


		int numPixels = width * height;
		canvas = new Canvas(width, height);
		unsigned char *dstloc = canvas->GetPixelBuffer();
		unsigned char red, green, blue;
		for (int i = 0; i < numPixels; ++i) {
			file.read((char*)buffer, 3);
			*dstloc = buffer[0];
			++dstloc;
			*dstloc = buffer[1];
			++dstloc;
			*dstloc = buffer[2];
			++dstloc;
			*dstloc = 255;
			++dstloc;
		}
		return true;
	}

	bool Image::CreateFromPpmAscii(const std::string& fileName)
	{
		return false;
	}

	bool Image::SaveAsPng(const std::string& fileName)
	{
		Canvas* canvas = GetCanvas();
		int pitch = canvas->GetPitch();
		unsigned char* loc;

		unsigned char* png_data = new unsigned char[canvas->GetPixelDataLength()];
		int  height = canvas->GetHeight();
		int  width = canvas->GetWidth();
		unsigned char* pixels = canvas->GetPixelBuffer();

		int i = 0;
		for (int y = 0; y < height; ++y)
		{
			for (int x = 0; x < width; ++x)
			{
				loc = pixels + pitch * y + x * 4;
				int b = *loc;
				int g = *(loc + 1);
				int r = *(loc + 2);
				int a = *(loc + 3);
				png_data[i] = r;
				png_data[i + 1] = g;
				png_data[i + 2] = b;
				png_data[i + 3] = a;
				i += 4;
			}
		}
		stbi_write_png(fileName.c_str(), width, height, 4, png_data, pitch);
		delete[] png_data;
		return true;
	}
}

