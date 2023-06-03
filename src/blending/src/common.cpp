#include "common.h"

double pixel_brightness(Vec4b& v)
{
    return sqrt(pow(v[0], 2) + pow(v[1], 2) + pow(v[2], 2));
}

void in_place_sort(vector<PixelBrightnessMeasure>& original) {
    sort(original.begin(), original.end(), less_than_key());
}