#include "blending_common.h"

double pixel_brightness(uchar r, uchar g, uchar b)
{
     return sqrt(pow(r, 2) + pow(g, 2) + pow(b, 2));
}