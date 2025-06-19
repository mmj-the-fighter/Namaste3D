#include <iostream>
#include <cstring>
#include "spn_canvas.h"
#include "spn_image.h"
#include "spn_rfont.h"
#include "../common/spn_utils.h"

namespace spn 
{
	Canvas::Canvas(unsigned int aWidth, unsigned int aHeight)
		:width(aWidth), height(aHeight), lastFrameTime(0.0f), font(nullptr),
		primaryColorR(255), primaryColorG(255), primaryColorB(255),
		clearColorR(0), clearColorG(0), clearColorB(0), isAlphaBlendingEnabled(false)
	{
		channels = 4;
		pitch = width * 4;
		aspectRatio = static_cast<float>(width) / static_cast<float>(height);
		numOfPixels = aWidth * aHeight;
		pixelDataLength = numOfPixels * 4;
		pixBuffer = new unsigned char[pixelDataLength];
	}

	Canvas::~Canvas(){
		if (pixBuffer != nullptr) {
			delete[] pixBuffer;
			pixBuffer = nullptr;
		}
		if (font != nullptr){
			delete font;
			font = nullptr;
		}
	}

	Canvas* Canvas::Clone(){
		Canvas* newCanvas = new Canvas(width, height);
		if (pixBuffer != nullptr) {
			memcpy(newCanvas->GetPixelBuffer(), GetPixelBuffer(), pixelDataLength);
		}
		if (font != nullptr){
			RFont* newFont = font->Clone();
			newCanvas->SetFont(newFont);
		}
		return newCanvas;
	}

	//For debugging 
	void Canvas::FillAnything()
	{
		unsigned int half = numOfPixels * 4 / 2;
		for (unsigned int i = 0; i < numOfPixels * 4; i += 4) {
			if (i < half){
				pixBuffer[i] = 128;
				pixBuffer[i + 1] = 0;
				pixBuffer[i + 2] = 0;
				pixBuffer[i + 3] = 255;
			}
			else{
				pixBuffer[i] = 0;
				pixBuffer[i + 1] = 255;
				pixBuffer[i + 2] = 0;
				pixBuffer[i + 3] = 255;
			}
		}
	}

	void Canvas::FlipHorizontally() 
	{
		for (int y = 0; y < height; ++y)
		{
			for (int x = 0; x < width/2; ++x)
			{
				unsigned char* aLoc = pixBuffer + pitch * y + x * channels;
				unsigned char* bLoc = pixBuffer + pitch * y + (width-1-x) * channels;

				unsigned char tBlue = *aLoc;
				unsigned char tGreen = *(aLoc+1);
				unsigned char tRed = *(aLoc+2);
				unsigned char tAlpha = *(aLoc+3);

				*aLoc = *bLoc;
				*(aLoc+1) = *(bLoc+1);
				*(aLoc+2) = *(bLoc+2);
				*(aLoc+3) = *(bLoc+3);

				*bLoc = tBlue;
				*(bLoc + 1) = tGreen;
				*(bLoc + 2) = tRed;
				*(bLoc + 3) = tAlpha;
			}
		}
	}

	void Canvas::FlipVertically() 
	{
		
		for (int x = 0; x < width; ++x)
		{
			for (int y = 0; y < height/2; ++y)
			{
				unsigned char* aLoc = pixBuffer + pitch * y + x * channels;
				unsigned char* bLoc = pixBuffer + pitch * (height-1-y) + x * channels;

				unsigned char tBlue = *aLoc;
				unsigned char tGreen = *(aLoc + 1);
				unsigned char tRed = *(aLoc + 2);
				unsigned char tAlpha = *(aLoc + 3);

				*aLoc = *bLoc;
				*(aLoc + 1) = *(bLoc + 1);
				*(aLoc + 2) = *(bLoc + 2);
				*(aLoc + 3) = *(bLoc + 3);

				*bLoc = tBlue;
				*(bLoc + 1) = tGreen;
				*(bLoc + 2) = tRed;
				*(bLoc + 3) = tAlpha;
			}
		}
	}

	void Canvas::Clear()
	{
		for (unsigned int i = 0; i < numOfPixels*4; i += 4) {
			pixBuffer[i] = clearColorB;
			pixBuffer[i + 1] = clearColorG;
			pixBuffer[i + 2] = clearColorR;
			pixBuffer[i + 3] = 255;
		}
	}

	void Canvas::DrawLine(int x0, int y0, int x1, int y1)
	{
		float x, y, xIncr, yIncr;
		int steps;
		int dx = x1 - x0;
		int dy = y1 - y0;

		if (abs(dx) > abs(dy)) {
			steps = abs(dx);
		}
		else {
			steps = abs(dy);
		}
		xIncr = dx / static_cast<float>(steps);
		yIncr = dy / static_cast<float>(steps);
		x = x0;
		y = y0;
		for (int i = 0; i < steps; i++) {
			int ix = static_cast<int>(x + 0.5f);
			int iy = static_cast<int>(y + 0.5f);
			unsigned char* dstLoc = pixBuffer + pitch * iy + ix * channels;
			*dstLoc++ = primaryColorB;
			*dstLoc++ = primaryColorG;
			*dstLoc++ = primaryColorR;
			*dstLoc = 255;
			x += xIncr;
			y += yIncr;
		}
	}

	void Canvas::DrawFilledRectangularRegion(int left, int top, int right, int bottom) {

		unsigned char* loc;
		int x, y;

		//clip points
		//left
		if (left >= width) {
			left = width - 1;
		}
		else if (left < 0) {
			left = 0;
		}
		//right
		if (right >= width) {
			right = width - 1;
		}
		else if (right < 0) {
			right = 0;
		}
		//top
		if (top >= height) {
			top = height - 1;
		}
		else if (top < 0) {
			top = 0;
		}
		//bottom
		if (bottom >= height) {
			bottom = height - 1;
		}
		else if (bottom < 0) {
			bottom = 0;
		}

		for (y = top; y <= bottom; ++y)
		{
			for (x = left; x <= right; ++x)
			{
				loc = pixBuffer + pitch * y + x * channels;
				*loc++ = primaryColorB;
				*loc++ = primaryColorG;
				*loc++ = primaryColorR;
				*loc = 255;
			}
		}
	}

	void Canvas::DrawImage(Image* image, int x, int y)
	{
		Canvas *imCanvas = image->GetCanvas();
		BitBlockTransfer(imCanvas->GetPixelBuffer(),
			imCanvas->GetWidth(),
			imCanvas->GetHeight(),
			x, y);
		//CopyAllPixels(imCanvas->GetWidth(),
		//	imCanvas->GetHeight(),
		//	imCanvas->GetPixelBuffer(),
		//	x, y);
	}

	void Canvas::DrawSubImage(Image* image, int x, int y,
		int tileStartX, int tileStartY,
		int tileWidth, int tileHeight
	) {
		Canvas* imCanvas = image->GetCanvas();

		BitBlockTransfer(imCanvas->GetPixelBuffer(),
			imCanvas->GetWidth(),
			imCanvas->GetHeight(),
			x, y, tileStartX, tileStartY, tileWidth, tileHeight);


	}

	void Canvas::DrawImageChromaKeyed(Image* image,
		int x, int y,
		int chromaR, int chromaG, int chromaB)
	{
		Canvas *imCanvas = image->GetCanvas();
		BitBlockTransfer(imCanvas->GetPixelBuffer(),
			imCanvas->GetWidth(),
			imCanvas->GetHeight(), x, y, 0, 0, -1, -1, 
			chromaR, chromaG, chromaB);
		//CopyPixels(
		//	0,
		//	0,
		//	imCanvas->GetWidth(),
		//	imCanvas->GetHeight(),
		//	imCanvas->GetWidth(),
		//	imCanvas->GetHeight(),
		//	imCanvas->GetPixelBuffer(),
		//	x,
		//	y,
		//	chromaR,
		//	chromaG,
		//	chromaB
		//	);

	}

	void Canvas::DrawSubImageChromaKeyed(Image* image,
		int x, int y,
		int tileStartX, int tileStartY,
		int tileWidth, int tileHeight,
		int chromaR, int chromaG, int chromaB
	) {
		Canvas* imCanvas = image->GetCanvas();

		BitBlockTransfer(imCanvas->GetPixelBuffer(),
			imCanvas->GetWidth(),
			imCanvas->GetHeight(),
			x, y, tileStartX, tileStartY, tileWidth, tileHeight,
			chromaR, chromaG, chromaB);

		//CopyPixels(
		//	tileStartX,
		//	tileStartY,
		//	tileWidth,
		//	tileHeight,
		//	imCanvas->GetWidth(),
		//	imCanvas->GetHeight(),
		//	imCanvas->GetPixelBuffer(),
		//	x,
		//	y,
		//	chromaR,
		//	chromaG,
		//	chromaB
		//);

	}

	void Canvas::DrawText(const char* text, int x, int y)
	{
		if (font == nullptr) {
			return;
		}
		int atlasWidth = font->GetCanvas()->GetWidth();
		int atlasHeight = font->GetCanvas()->GetHeight();
		int cellWidth = font->GetCellWidth();
		int cellHeight = font->GetCellHeight();
		int maxCols = atlasWidth / cellWidth;
		unsigned char baseChar = font->GetBaseChar();
		unsigned char* srcPixels = font->GetCanvas()->GetPixelBuffer();
		if (srcPixels == nullptr){
			return;
		}
		
		
		int xpos = x;
		int i = 0;
		unsigned char c;
		while ((c = text[i]) != '\0') {
			if (c != ' ') {
				unsigned char charDiff = c - baseChar;
				int row = charDiff / maxCols;
				int col = charDiff - (row*maxCols);
				int atlasX = col * cellWidth;
				int atlasY = row * cellHeight;
				//draw a region of atlas at xpos
				BitBlockTransfer(srcPixels, atlasWidth, atlasHeight,
					xpos, y, atlasX, atlasY, cellWidth,
					cellHeight, 0, 0, 0, primaryColorR,
					primaryColorG, primaryColorB);

				//CopyPixels(atlasX, atlasY, cellWidth, cellHeight,
				//	atlasWidth, atlasHeight, srcPixels, xpos, y,
				//	0, 0, 0,
				//	primaryColorR,
				//	primaryColorG,
				//	primaryColorB
				//	);

			}
			xpos += font->GetCharWidth(c);
			++i;
		}


	}

	void Canvas::DrawString(const std::string& str, int x, int y)
	{
		if (font == nullptr) {
			return;
		}
		int atlasWidth = font->GetCanvas()->GetWidth();
		int atlasHeight = font->GetCanvas()->GetHeight();
		int cellWidth = font->GetCellWidth();
		int cellHeight = font->GetCellHeight();
		int maxCols = atlasWidth / cellWidth;
		unsigned char baseChar = font->GetBaseChar();
		unsigned char* srcPixels = font->GetCanvas()->GetPixelBuffer();
		if (srcPixels == nullptr){
			return;
		}


		int xpos = x;
		for (int i = 0; i < str.size(); ++i) {
			unsigned char c = str.at(i);
			if (c != ' ') {
				unsigned char charDiff = c - baseChar;
				int row = charDiff / maxCols;
				int col = charDiff - (row*maxCols);
				int atlasX = col * cellWidth;
				int atlasY = row * cellHeight;
				//draw a region of atlas at xpos

				BitBlockTransfer(srcPixels, atlasWidth, atlasHeight,
					xpos, y, atlasX, atlasY, cellWidth,
					cellHeight, 0, 0, 0, primaryColorR,
					primaryColorG, primaryColorB);

				//CopyPixels(atlasX, atlasY, cellWidth, cellHeight,
				//	atlasWidth, atlasHeight, srcPixels, xpos, y,
				//	0, 0, 0,
				//	primaryColorR,
				//	primaryColorG,
				//	primaryColorB
				//	);


			}
			xpos += font->GetCharWidth(c);
		}


	}

	void Canvas::SetAlpha(float alpha) {
		int ialpha = alpha * 255.0;
		for (int y = 0; y < height; ++y) {
			for (int x = 0; x < width; ++x) {
				unsigned char* loc = pixBuffer + pitch * y + x * 4;
				*(loc + 3) = ialpha;
			}
		}
	}

	void Canvas::BitBlockTransfer(
		unsigned char* srcPixels,
		int srcTotalWidth,
		int srcTotalHeight,
		int dstXStart,
		int dstYStart,
		int srcXStart,
		int srcYStart,
		int srcWidth,
		int srcHeight,
		int chromaR,
		int chromaG,
		int chromaB,
		int newColorR,
		int newColorG,
		int newColorB
	) {
#define ONEOVER255 0.00392156862745098f
		bool isSourceCropped = srcWidth > 0 && srcHeight > 0;
		if (!isSourceCropped) {
			srcWidth = srcTotalWidth;
			srcHeight = srcTotalHeight;
		}
		int dstX, dstY;
		int numOfChannels = channels;
		unsigned char* dstPixels = pixBuffer;
		unsigned char* dstLoc, * srcLoc;
		int srcPitch = srcTotalWidth * numOfChannels;
		int dstWidth = width;
		int dstHeight = height;
		int dstPitch = pitch;
		int x, y;
		

		/* culling */
		if (
			(dstXStart + srcWidth < 0 || dstXStart >= dstWidth) ||
			(dstYStart + srcHeight < 0 || dstYStart >= dstHeight)
			)
		{
			return;
		}


		/*compute intersection of e.g.: r1(image) with r2(screen) */
		Rect r1;
		r1.left = dstXStart;
		r1.top = dstYStart;
		r1.width = srcWidth;
		r1.height = srcHeight;

		Rect r2;
		r2.left = 0;
		r2.top = 0;
		r2.width = dstWidth;
		r2.height = dstHeight;

		Rect ir;
		FindRectToRectIntersection(r1, r2, ir);


		/*clip new */
		int sx, sy, tw, th;
		int srcX, srcY;
		sx = ir.left - r1.left;
		sy = ir.top - r1.top;

		//correction
		tw = ir.width;
		th = ir.height;

		bool fColorReplace = newColorR >= 0 && newColorG >= 0 && newColorR >= 0;
		bool fChromaKeyed = chromaR >= 0 && chromaG >= 0 && chromaB >= 0;

		/*iterate through source rectangle * /
		/* copy to destination */
		for (srcY = srcYStart + sy, y = 0; srcY < (srcYStart + sy + th); ++srcY, ++y)
		{
			for (srcX = srcXStart + sx, x = 0; srcX < (srcXStart + sx + tw); ++srcX, ++x)
			{
				dstX = ir.left + x;
				dstY = ir.top + y;
				srcLoc = srcPixels + srcPitch * srcY + srcX * numOfChannels;
				dstLoc = dstPixels + dstPitch * dstY + dstX * numOfChannels;
				unsigned char sB = *srcLoc;
				unsigned char sG = *(srcLoc + 1);
				unsigned char sR = *(srcLoc + 2);
				unsigned char sA = *(srcLoc + 3);
				bool dontDraw = fChromaKeyed && chromaB == sB && chromaG == sG && chromaR == sR;
				if (dontDraw) {
					continue;
				}
				if (fColorReplace) {
					sB = newColorB;
					sG = newColorG;
					sR = newColorR;
				}
				if (!isAlphaBlendingEnabled) {
					*dstLoc++ = sB;
					*dstLoc++ = sG;
					*dstLoc++ = sR;
					*dstLoc = sA;
					srcLoc += 4;
				}
				else {
					float srcAlpha = sA * ONEOVER255;
					float oneMinusSrcAlpha = 1.0f - srcAlpha;
					float b = srcAlpha * sB + oneMinusSrcAlpha * (*dstLoc);
					float g = srcAlpha * sG + oneMinusSrcAlpha * (*(dstLoc + 1));
					float r = srcAlpha * sR + oneMinusSrcAlpha * (*(dstLoc + 2));
					*dstLoc++ = static_cast<unsigned char> (b);
					*dstLoc++ = static_cast<unsigned char> (g);
					*dstLoc++ = static_cast<unsigned char> (r);
					*dstLoc = 255;
				}
			}
		}
#undef ONEOVER255
	}


	//void Canvas::CopyAllPixels(
	//	int srcWidth, int srcHeight,
	//	unsigned char * srcPixels,
	//	int dstXStart, int dstYStart)
	//{
	//	Rect r1, r2, ir;
	//	int sx, sy, tw, th;
	//	int srcX, srcY;
	//	int dstX, dstY;
	//	int numOfChannels = channels;
	//	unsigned char *dstPixels = pixBuffer;
	//	unsigned char *dstLoc, *srcLoc;
	//	int srcPitch = srcWidth * numOfChannels;
	//	int dstWidth = width;
	//	int dstHeight = height;
	//	int dstPitch = pitch;
	//	int x, y;
	//	double oneOver255 = 1.0 / 255.0;
	//	unsigned char srcAlphaUchar;



	//	/* culling */
	//	if (
	//		(dstXStart + srcWidth < 0 || dstXStart >= dstWidth) ||
	//		(dstYStart + srcHeight < 0 || dstYStart >= dstHeight)
	//		)
	//	{
	//		return;
	//	}

	//	/*compute intersection*/
	//	r1.left = dstXStart;
	//	r1.top = dstYStart;
	//	r1.width = srcWidth;
	//	r1.height = srcHeight;

	//	r2.left = 0;
	//	r2.top = 0;
	//	r2.width = dstWidth;
	//	r2.height = dstHeight;

	//	IntersctRectRect(r1, r2, ir);


	//	/*clip new */
	//	sx = ir.left - r1.left;
	//	sy = ir.top - r1.top;

	//	//correction
	//	tw = ir.width;
	//	th = ir.height;


	//	/*iterate through source rectangle * /
	//	/* copy to destination */
	//	for (srcY = sy, y = 0; srcY < (sy + th); ++srcY, ++y)
	//	{
	//		for (srcX = sx, x = 0; srcX < (sx + tw); ++srcX, ++x)
	//		{
	//			dstX = ir.left + x;
	//			dstY = ir.top + y;
	//			srcLoc = srcPixels + srcPitch * srcY + srcX * numOfChannels;
	//			dstLoc = dstPixels + dstPitch * dstY + dstX * numOfChannels;
	//			*dstLoc++ = *srcLoc++;
	//			*dstLoc++ = *srcLoc++;
	//			*dstLoc++ = *srcLoc++;
	//			*dstLoc = *srcLoc;
	//		}
	//	}

	//}

	//void Canvas::CopyPixels(
	//	int srcXStart,
	//	int srcYStart,
	//	int srcWidth,
	//	int srcHeight,
	//	int srcTotalWidth,
	//	int srcTotalHeight,
	//	unsigned char * srcPixels,
	//	int dstXStart,
	//	int dstYStart)
	//{
	//	Rect r1, r2, ir;
	//	int sx, sy, tw, th;
	//	int srcX, srcY;
	//	int dstX, dstY;
	//	int numOfChannels = channels;
	//	unsigned char *dstPixels = pixBuffer;
	//	unsigned char *dstLoc, *srcLoc;
	//	int srcPitch = srcTotalWidth * numOfChannels;
	//	int dstWidth = width;
	//	int dstHeight = height;
	//	int dstPitch = pitch;
	//	int x, y;


	//	/* culling */
	//	if (
	//		(dstXStart + srcWidth < 0 || dstXStart >= dstWidth) ||
	//		(dstYStart + srcHeight < 0 || dstYStart >= dstHeight)
	//		)
	//	{
	//		return;
	//	}

	//	/*compute intersection*/
	//	r1.left = dstXStart;
	//	r1.top = dstYStart;
	//	r1.width = srcWidth;
	//	r1.height = srcHeight;

	//	r2.left = 0;
	//	r2.top = 0;
	//	r2.width = dstWidth;
	//	r2.height = dstHeight;

	//	IntersctRectRect(r1, r2, ir);


	//	/*clip new */
	//	sx = ir.left - r1.left;
	//	sy = ir.top - r1.top;

	//	//correction
	//	tw = ir.width;
	//	th = ir.height;


	//	/*iterate through source rectangle * /
	//	/* copy to destination */
	//	for (srcY = srcYStart + sy, y = 0; srcY < (srcYStart+sy + th); ++srcY, ++y)
	//	{
	//		for (srcX = srcXStart + sx, x = 0; srcX < (srcXStart+sx + tw); ++srcX, ++x)
	//		{
	//			dstX = ir.left + x;
	//			dstY = ir.top + y;
	//			srcLoc = srcPixels + srcPitch * srcY + srcX * numOfChannels;
	//			dstLoc = dstPixels + dstPitch * dstY + dstX * numOfChannels;
	//			*dstLoc++ = *srcLoc++;
	//			*dstLoc++ = *srcLoc++;
	//			*dstLoc++ = *srcLoc++;
	//			*dstLoc = *srcLoc;
	//		}
	//	}
	//}


	//void Canvas::CopyPixels(
	//	int srcXStart,
	//	int srcYStart,
	//	int srcWidth,
	//	int srcHeight,
	//	int srcTotalWidth,
	//	int srcTotalHeight,
	//	unsigned char * srcPixels,
	//	int dstXStart,
	//	int dstYStart,
	//	int chromaR,
	//	int chromaG,
	//	int chromaB
	//	)
	//{
	//	Rect r1, r2, ir;
	//	int sx, sy, tw, th;
	//	int srcX, srcY;
	//	int dstX, dstY;
	//	int numOfChannels = channels;
	//	unsigned char *dstPixels = pixBuffer;
	//	unsigned char *dstLoc, *srcLoc;
	//	int srcPitch = srcTotalWidth * numOfChannels;
	//	int dstWidth = width;
	//	int dstHeight = height;
	//	int dstPitch = pitch;
	//	int x, y;



	//	/* culling */
	//	if (
	//		(dstXStart + srcWidth < 0 || dstXStart >= dstWidth) ||
	//		(dstYStart + srcHeight < 0 || dstYStart >= dstHeight)
	//		)
	//	{
	//		return;
	//	}

	//	/*compute intersection*/
	//	r1.left = dstXStart;
	//	r1.top = dstYStart;
	//	r1.width = srcWidth;
	//	r1.height = srcHeight;

	//	r2.left = 0;
	//	r2.top = 0;
	//	r2.width = dstWidth;
	//	r2.height = dstHeight;

	//	IntersctRectRect(r1, r2, ir);


	//	/*clip new */
	//	sx = ir.left - r1.left;
	//	sy = ir.top - r1.top;

	//	//correction
	//	tw = ir.width;
	//	th = ir.height;


	//	/*iterate through source rectangle * /
	//	/* copy to destination */
	//	for (srcY = srcYStart + sy, y = 0; srcY < (srcYStart + sy + th); ++srcY, ++y)
	//	{
	//		for (srcX = srcXStart + sx, x = 0; srcX < (srcXStart + sx + tw); ++srcX, ++x)
	//		{
	//			dstX = ir.left + x;
	//			dstY = ir.top + y;
	//			srcLoc = srcPixels + srcPitch * srcY + srcX * numOfChannels;
	//			dstLoc = dstPixels + dstPitch * dstY + dstX * numOfChannels;
	//			if (chromaB != *srcLoc ||
	//				chromaG != *(srcLoc + 1) ||
	//				chromaR != *(srcLoc + 2)
	//				)
	//			{
	//				*dstLoc++ = *srcLoc++;
	//				*dstLoc++ = *srcLoc++;
	//				*dstLoc++ = *srcLoc++;
	//				*dstLoc = *srcLoc;
	//			}
	//		}
	//	}
	//}

	//void Canvas::CopyPixels(
	//	int srcXStart,
	//	int srcYStart,
	//	int srcWidth,
	//	int srcHeight,
	//	int srcTotalWidth,
	//	int srcTotalHeight,
	//	unsigned char * srcPixels,
	//	int dstXStart,
	//	int dstYStart,
	//	int chromaR,
	//	int chromaG,
	//	int chromaB,
	//	int textColorR,
	//	int textColorG,
	//	int textColorB
	//	)
	//{
	//	Rect r1, r2, ir;
	//	int sx, sy, tw, th;
	//	int srcX, srcY;
	//	int dstX, dstY;
	//	int numOfChannels = channels;
	//	unsigned char *dstPixels = pixBuffer;
	//	unsigned char *dstLoc, *srcLoc;
	//	int srcPitch = srcTotalWidth * numOfChannels;
	//	int dstWidth = width;
	//	int dstHeight = height;
	//	int dstPitch = pitch;
	//	int x, y;



	//	/* culling */
	//	if (
	//		(dstXStart + srcWidth < 0 || dstXStart >= dstWidth) ||
	//		(dstYStart + srcHeight < 0 || dstYStart >= dstHeight)
	//		)
	//	{
	//		return;
	//	}

	//	/*compute intersection*/
	//	r1.left = dstXStart;
	//	r1.top = dstYStart;
	//	r1.width = srcWidth;
	//	r1.height = srcHeight;

	//	r2.left = 0;
	//	r2.top = 0;
	//	r2.width = dstWidth;
	//	r2.height = dstHeight;

	//	IntersctRectRect(r1, r2, ir);


	//	/*clip new */
	//	sx = ir.left - r1.left;
	//	sy = ir.top - r1.top;

	//	//correction
	//	tw = ir.width;
	//	th = ir.height;


	//	/*iterate through source rectangle * /
	//	/* copy to destination */
	//	for (srcY = srcYStart + sy, y = 0; srcY < (srcYStart + sy + th); ++srcY, ++y)
	//	{
	//		for (srcX = srcXStart + sx, x = 0; srcX < (srcXStart + sx + tw); ++srcX, ++x)
	//		{
	//			dstX = ir.left + x;
	//			dstY = ir.top + y;
	//			srcLoc = srcPixels + srcPitch * srcY + srcX * numOfChannels;
	//			dstLoc = dstPixels + dstPitch * dstY + dstX * numOfChannels;
	//			if (chromaB != *srcLoc ||
	//				chromaG != *(srcLoc + 1) ||
	//				chromaR != *(srcLoc + 2)
	//				)
	//			{
	//				*dstLoc++ = textColorB;
	//				*dstLoc++ = textColorG;
	//				*dstLoc++ = textColorR;
	//				*dstLoc = 255;
	//			}
	//		}
	//	}
	//}
}
