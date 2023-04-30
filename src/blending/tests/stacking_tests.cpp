#include <gtest/gtest.h>
#include "MeanStacker.h"
#include "MedianStacker.h"
#include "loaders.h"
#include <opencv2/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <filesystem>

using namespace common::tests;

TEST(LongExposures, MeanShortExposure){

  std::string folder = "./src/blending/tests/long_exposure/5Photos_10Seconds";
  MeanStacker m;
  for (const auto & entry : filesystem::directory_iterator(folder))
  {
      Mat img = loadStdImage(entry.path().c_str());
      m.AddToStack(img);
  }   

  auto total_image = m.GetCurrentBlend();
        
  imwrite("./src/blending/tests/long_exposure_tests/5Photos_10Seconds_mean.png", total_image);
}


TEST(LongExposures, MedianShortExposure){

  std::string folder = "./src/blending/tests/long_exposure/5Photos_10Seconds";
  MedianStacker m(4496, 3000);
  for (const auto & entry : filesystem::directory_iterator(folder))
  {
      Mat img = loadStdImage(entry.path().c_str());
      m.AddToStack(img);
  }   

  auto total_image = m.GetCurrentBlend();
        
  imwrite("./src/blending/tests/long_exposure_results/5Photos_10Seconds_median.png", total_image);
}