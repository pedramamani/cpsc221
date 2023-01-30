#ifndef _FILLER_H_
#define _FILLER_H_

#include "cs221util/PNG.h"
#include "stack.h"
#include "queue.h"
#include "animation.h"

#include "rainbowColorPicker.h"
#include "borderColorPicker.h"
#include "stripeColorPicker.h"
#include "colorPicker.h"
using namespace cs221util;

namespace filler
{
    animation fillStripeDFS(PNG& img, int x, int y, HSLAPixel fillColor,
                       int stripeSpacing, double tolerance, int frameFreq);

    animation fillBorderDFS(PNG& img, int x, int y, HSLAPixel borderColor,
            double tolerance, int frameFreq);

    animation fillRainDFS(PNG& img, int x, int y,
        long double freq, double tolerance, int frameFreq);

    animation fillStripeBFS(PNG& img, int x, int y, HSLAPixel fillColor,
            int stripeSpacing, double tolerance, int frameFreq);

    animation fillBorderBFS(PNG& img, int x, int y, HSLAPixel borderColor,
            double tolerance, int frameFreq);

    animation fillRainBFS(PNG& img, int x, int y,
            long double freq, double tolerance, int frameFreq);

    template <template <class T> class OrderingStructure>
    animation fill(PNG& img, int x, int y, colorPicker& fillColor,
                   double tolerance, int frameFreq);
}

#include "filler.cpp"
#endif
