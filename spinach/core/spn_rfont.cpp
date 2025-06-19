#include <fstream>
#include <cstring>
#include <cstdlib>
#include <cctype>
#include "spn_rfont.h"

namespace spn
{
	RFont::RFont(const std::string& fontImageFileName, const std::string& fontCsvFileName)
		:isInitSuccess(false)
	{
		isInitSuccess = Create(fontImageFileName, fontCsvFileName);
	}
	bool RFont::Create(const std::string& fontImageFileName, const std::string& fontCsvFileName)
	{
		image = new spn::Image();
		bool  rv = image->CreateFromPpmRaw(fontImageFileName);
		if (false == rv){
			delete image;
			return false;
		}
		std::ifstream inputFile(fontCsvFileName);
		if (!inputFile.is_open()) {
			delete image;
			return false;
		}

		std::string line;
		int lineNum = 0;
		char key[1024];
		char value[1024];
		int charIndex = 0;
		
		while (getline(inputFile, line)) 
		{
			int lineLength = line.size();
			++lineNum;
			if (lineNum == 1 || lineNum == 2 || lineNum == 6) {
				continue;
			}

			if (lineNum < 9) {
				//read key
				int i = 0;
				int k = 0;
				while (line[i] != ',') {
					key[k] = line[i];
					++k;
					++i;
				}
				key[k] = '\0';

				//read value
				++i;
				k = 0;
				while (i < lineLength) {
					value[k] = line[i];
					++k;
					++i;
				}
				value[k] = '\0';

				if ( !strcmp("Cell Width", key) ) {
					cellWidth = atoi(value);
				}
				else if( !strcmp("Cell Height", key) ) {
					cellHeight = atoi(value);
				}
				else if (!strcmp("Base Char", key)) {
					baseChar = atoi(value);
				}
				else if (!strcmp("Font Height", key)) {
					fontHeight = atoi(value);
				}
			}
			else  if(charIndex < 256){
				//skip key
				int i = 0;
				while (line[i] != ',') {
					++i;
				}
				//read value
				++i;
				int k = 0;
				while (i < lineLength) {
					value[k] = line[i];
					++k;
					++i;
				}
				value[k] = '\0';
				fontWidth[charIndex] = atoi(value);
				++charIndex;
			}
		}
		inputFile.close();
		return true;
	}

	void RFont::Destroy(){
		if (image != nullptr) {
			delete image;
		}
	}
	RFont* RFont::Clone(){
		Image* newImage = nullptr;
		if (image != nullptr) {
			newImage = image->Clone();
		}
		RFont* newFont = new RFont(*this, newImage);
		return newFont;
	}

	RFont::RFont(RFont& cf, spn::Image *im){
		this->baseChar = cf.baseChar;
		this->cellHeight = cf.cellHeight;
		this->cellWidth = cf.cellWidth;
		this->fontHeight = cf.fontHeight;
		for (int i = 0; i < 256; i++) {
			this->fontWidth[i] = cf.fontWidth[i];
		}
		this->image = im;
	}

}