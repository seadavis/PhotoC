#include "camera.h"

enum LongExposureBlendingMethod
{
    Average,
    Brighten
};

struct LongExposureDefinition
{
    LongExposureShots shots;
    LongExposureBlendingMethod method;

    LongExposureDefinition(LongExposureShots shots, LongExposureBlendingMethod method) : shots(shots), method(method){};
};