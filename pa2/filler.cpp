#include "filler.h"

animation filler::fillStripeDFS(PNG& img, int x, int y, HSLAPixel fillColor,
    int stripeSpacing, double tolerance, int frameFreq)
{
    stripeColorPicker a (fillColor, stripeSpacing);
    return fill<Stack>(img, x, y, a, tolerance, frameFreq);
}

animation filler::fillBorderDFS(PNG& img, int x, int y,
    HSLAPixel borderColor, double tolerance, int frameFreq)
{
    borderColorPicker a (borderColor, img, tolerance, *img.getPixel(x,y));
    return fill<Stack>(img, x, y, a, tolerance, frameFreq);
}

animation filler::fillRainDFS(PNG& img, int x, int y,
    long double freq, double tolerance, int frameFreq)
{
    rainbowColorPicker a (freq);
    return fill<Stack>(img, x, y, a, tolerance, frameFreq);
}

animation filler::fillStripeBFS(PNG& img, int x, int y, HSLAPixel fillColor,
    int stripeSpacing, double tolerance, int frameFreq)
{
    stripeColorPicker a (fillColor, stripeSpacing);
    return fill<Queue>(img, x, y, a, tolerance, frameFreq);
}

animation filler::fillBorderBFS(PNG& img, int x, int y,
    HSLAPixel borderColor, double tolerance, int frameFreq)
{
    borderColorPicker a (borderColor, img, tolerance, *img.getPixel(x,y));
    return fill<Queue>(img, x, y, a, tolerance, frameFreq);
}

animation filler::fillRainBFS(PNG& img, int x, int y,
    long double freq, double tolerance, int frameFreq)
{
    rainbowColorPicker a (freq);
    return fill<Queue>(img, x, y, a, tolerance, frameFreq);
}

template <template <class T> class OrderingStructure>
animation filler::fill(PNG& img, int x, int y, colorPicker& fillColor,
    double tolerance, int frameFreq)
{
    animation anim;
    int count = 0;

    OrderingStructure<pair<int, int>> line;
    set<pair<int, int>> visited;
    pair<int, int> removed;
    pair<int, int> order[8] = {
        make_pair(1, -1), make_pair(0, -1), make_pair(-1, -1), make_pair(-1, 0),
        make_pair(-1, 1), make_pair(0, 1), make_pair(1, 1), make_pair(1, 0)};
    HSLAPixel center = *img.getPixel(x, y);

    if (x >= 0 && x <= (int) img.width() - 1 && y >= 0 && y <= (int) img.height() - 1 &&
    img.getPixel(x,y)->dist(center) <= tolerance &&
    visited.find(pair<int, int>(x,y)) == visited.end()) {
        *img.getPixel(x, y) = fillColor(x, y);
        line.add(pair<int, int>(x,y));
        visited.insert(pair<int, int>(x,y));
        count++;
        if (count % frameFreq == 0)
            anim.addFrame(img);
    }

    while (!line.isEmpty()) {
        removed = line.remove();
        for (pair<int, int> p : order) {
            x = removed.first + p.first;
            y = removed.second + p.second;
            if (x >= 0 && x <= (int) img.width() - 1 && y >= 0 && y <= (int) img.height() - 1 &&
            img.getPixel(x,y)->dist(center) <= tolerance &&
            visited.find(pair<int, int>(x,y)) == visited.end()) {
                *img.getPixel(x, y) = fillColor(x, y);
                line.add(pair<int, int>(x,y));
                visited.insert(pair<int, int>(x,y));
                count++;
                if (count % frameFreq == 0)
                    anim.addFrame(img);
            }
        }
    }

    anim.addFrame(img);
    return anim;
}
