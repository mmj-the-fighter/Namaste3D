#ifndef _SPN_UTILS_H_
#define _SPN_UTILS_H_
#include <string>
namespace spn
{
	typedef struct _Rect
	{
		int left;
		int top;
		int width;
		int height;
	}Rect;
	void FindRectToRectIntersection(const Rect& r1, const Rect& r2, Rect& ir);
	std::string GetTimeBasedScreenShotFileName();
}

#endif

