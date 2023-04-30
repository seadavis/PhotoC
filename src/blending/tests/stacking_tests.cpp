#include <gtest/gtest.h>
#include "MeanStacker.h"
#include "MedianStacker.h"
#include "loaders.h"
#include <opencv2/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <string_view>
#include <filesystem>

using namespace common::tests;

constexpr string_view LongExposureDirectory = "./src/blending/tests/long_exposure/";
constexpr string_view LongExposureResultsDirectory = "./src/blending/tests/long_exposure_results/";

class LongExposureData :
    public testing::TestWithParam<string> {
};

INSTANTIATE_TEST_SUITE_P(LongExposures,
                         LongExposureData,
                         testing::Values("5Photos_10Seconds", "10Photos_5Seconds", "20Photos_2.5Seconds"));

TEST_P(LongExposureData, MeanExposure){

  auto args = GetParam();
  std::string folder = string(LongExposureDirectory) + args;
  MeanStacker m;
  for (const auto & entry : filesystem::directory_iterator(folder))
  {
      Mat img = loadStdImage(entry.path().c_str());
      m.AddToStack(img);
  }   

  auto total_image = m.GetCurrentBlend();
        
  imwrite(string(LongExposureResultsDirectory) + args + "_mean.png", total_image);
}


TEST_P(LongExposureData, MedianExposure){

  auto args = GetParam();
  std::string folder = string(LongExposureDirectory) + args;
  MedianStacker m(4496, 3000);
  for (const auto & entry : filesystem::directory_iterator(folder))
  {
      Mat img = loadStdImage(entry.path().c_str());
      m.AddToStack(img);
  }   

  auto total_image = m.GetCurrentBlend();
        
  imwrite(string(LongExposureResultsDirectory) + args + "_median.png", total_image);
}