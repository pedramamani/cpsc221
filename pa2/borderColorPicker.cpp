#include "borderColorPicker.h"

using std::pair;
using std::map;

borderColorPicker::borderColorPicker(HSLAPixel fillColor, PNG & img, double tolerance, HSLAPixel center)
{
	color = fillColor;
	im = img;
	tol = tolerance;
	ctr = center;
}

HSLAPixel borderColorPicker::operator()(int x, int y)
{
	PNGmap::iterator stored = prev.find(pair<int, int>(x, y));
    if (stored == prev.end()) {
        HSLAPixel next = nextColor(x, y);
        prev[pair<int, int>(x, y)] = next;
        return next;
    } else
        return stored->second;
}

HSLAPixel borderColorPicker::nextColor(int x0, int y0)
{
	bool colorPixel = false;

	if (x0 <= 2 || x0 >= (int) im.width() - 3 || y0 <= 2 || y0 >= (int) im.height() - 3)
		colorPixel = true;
	else
		for (int x = x0 - 3; x <= x0 + 3; x++) {
			for (int y = y0 - 3; y <= y0 + 3; y++)
				if ((x-x0)*(x-x0) + (y-y0)*(y-y0) <= 9 && im.getPixel(x, y)->dist(ctr) > tol) {
					colorPixel = true;
					break;
				}
			if (colorPixel)
				break;
		}

	if (colorPixel)
		return color;
	else
		return *im.getPixel(x0, y0);
}
