#include "customColorPicker.h"
#include <math.h>

using std::pair;
using std::map;

customColorPicker::customColorPicker(HSLAPixel fillColor, HSLAPixel spiralColor,
    int xCenter, int yCenter, int spiralSpacing)
{
    fill = fillColor;
    color = spiralColor;
    x0 = xCenter;
    y0 = yCenter;
	spacing = spiralSpacing;
}

HSLAPixel customColorPicker::operator()(int x, int y)
{
	PNGmap::iterator stored = prev.find(pair<int, int>(x, y));
    if (stored == prev.end()) {
        HSLAPixel next = nextColor(x, y);
        prev[pair<int, int>(x, y)] = next;
        return next;
    } else
        return stored->second;
}

HSLAPixel customColorPicker::nextColor(int x, int y)
{
    double spiral = (double) (x-x0) / (y-y0+0.01) * tan((sqrt((x-x0)*(x-x0)+(y-y0)*(y-y0))) / spacing) - 1;
	if (-1 <= spiral)
		return color;
    else
    	return fill;
}
