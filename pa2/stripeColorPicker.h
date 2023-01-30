#ifndef STRIPECOLORPICKER_H
#define STRIPECOLORPICKER_H

#include <stdint.h>
#include <map>
#include <utility>
#include "colorPicker.h"

class stripeColorPicker : public colorPicker
{
  public:
    stripeColorPicker(HSLAPixel fillColor, int stripeSpacing);
    virtual HSLAPixel operator()(int x, int y);

  private:
    HSLAPixel nextColor(int x);
    HSLAPixel color;
    int spacing;
    typedef std::map<std::pair<int, int>, HSLAPixel> PNGmap;
    PNGmap prev;
};

#endif
