#ifndef BORDERCOLORPICKER_H
#define BORDERCOLORPICKER_H

#include <stdint.h>
#include <map>
#include <utility>
#include "colorPicker.h"

class borderColorPicker : public colorPicker
{
  public:
    borderColorPicker(HSLAPixel fillColor, PNG & img, double tolerance, HSLAPixel center);
    virtual HSLAPixel operator()(int x, int y);

  private:
    HSLAPixel nextColor(int x, int y);
    HSLAPixel color;
    PNG im;
    double tol;
    HSLAPixel ctr;
    typedef std::map<std::pair<int, int>, HSLAPixel> PNGmap;
    PNGmap prev;
};

#endif
