#ifndef _SPN_RFONT_H_
#define _SPN_RFONT_H_
#include <string>
#include "spn_image.h"

namespace spn
{
	class RFont
	{
	public:
		RFont(const std::string& fontImageFileName, const std::string& fontCsvFileName);
		inline bool IsInitSucceded(){
			return isInitSuccess;
		}
		inline Canvas* GetCanvas(){
			return image->GetCanvas();
		}
		inline int GetCellWidth(){
			return cellWidth;
		}
		inline int GetCellHeight(){
			return cellHeight;
		}
		inline int GetCharWidth(unsigned char c){
			return fontWidth[c];
		}
		inline int GetCharHeight(){
			return fontHeight;
		}
		inline unsigned char GetBaseChar(){
			return baseChar;
		}
		RFont* Clone();
	private:
		RFont(RFont& c, spn::Image *image);
		spn::Image *image;
		unsigned char baseChar;
		int cellWidth;
		int cellHeight;
		int fontHeight;
		int fontWidth[256];
		bool isInitSuccess;
		bool Create(const std::string& fontImageFileName, const std::string& fontCsvFileName);
		void Destroy();
	};
}

#endif
