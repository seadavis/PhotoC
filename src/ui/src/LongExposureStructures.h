#include "blending_common.h"
#include "BrightenStacker.h"
#include "MeanStacker.h"
#include "camera.h"

#pragma once

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


class StackerFactory
{

    public:
        StackerFactory(){};

        shared_ptr<IStackImages> CreateStacker(const LongExposureDefinition& definition)
        {
            if(definition.method == LongExposureBlendingMethod::Average)
            {
                return make_shared<MeanStacker>();
            }
            else
            {
                return make_shared<BrightenStacker>();
            }
        }
};