#include "stripeColorPicker.h"

using std::pair;
using std::map;

stripeColorPicker::stripeColorPicker(HSLAPixel fillColor, int stripeSpacing)
{
	color = fillColor;
	spacing = stripeSpacing;
}

HSLAPixel stripeColorPicker::operator()(int x, int y)
{
	PNGmap::iterator stored = prev.find(pair<int, int>(x, y));
    if (stored == prev.end()) {
        HSLAPixel next = nextColor(x);
        prev[pair<int, int>(x, y)] = next;
        return next;
    } else
        return stored->second;
}

HSLAPixel stripeColorPicker::nextColor(int x)
{
	if (x % spacing == 0)
		return color;
    else
    	return HSLAPixel (0, 0, 1);
}
