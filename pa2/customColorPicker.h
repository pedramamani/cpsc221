#ifndef CUSTOMCOLORPICKER_H
#define CUSTOMCOLORPICKER_H

#include <stdint.h>
#include <map>
#include <utility>
#include "colorPicker.h"

class customColorPicker : public colorPicker
{
  public:
    customColorPicker(HSLAPixel fillColor, HSLAPixel spiralColor, 
        int xCenter, int yCenter, int spiralSpacing);
    virtual HSLAPixel operator()(int x, int y);

  private:
    HSLAPixel nextColor(int x, int y);

    HSLAPixel color;
    HSLAPixel fill;
    int x0;
    int y0;
    int spacing;
    typedef std::map<std::pair<int, int>, HSLAPixel> PNGmap;
    PNGmap prev;
};

#endif
