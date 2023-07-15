#include "blending_common.h"

double pixel_brightness(Vec4b& v)
{
    return pixel_brightness(v[0], v[1], v[2]);
}

double pixel_brightness(uchar r, uchar g, uchar b)
{
     return sqrt(pow(r, 2) + pow(g, 2) + pow(b, 2));
}

void in_place_sort(vector<PixelBrightnessMeasure>& original) {
    sort(original.begin(), original.end(), less_than_key());
}