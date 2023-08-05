#include <gtest/gtest.h>
#include "MeanStacker.h"
#include "BrightenStacker.h"
#include "common_tests.h"
#include "blending_common.h"
#include <opencv2/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <string_view>
#include <filesystem>

using namespace common::tests;

constexpr string_view LongExposureDirectory = "./src/blending/tests/long_exposure_data/";
constexpr string_view LongExposureTargetDirectory = "./src/blending/tests/target_long_exposure/";
constexpr string_view LongExposureTestResultsDirectory = "./src/blending/tests/test_long_exposure/";

class LongExposureData :
    public testing::TestWithParam<string> {
};

INSTANTIATE_TEST_SUITE_P(LongExposures,
                         LongExposureData,
                         testing::Values("5Photos_10Seconds", "10Photos_5Seconds", "20Photos_2.5Seconds", "90Photos_2Seconds_Trails"));

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

  auto resultPath = string(LongExposureTestResultsDirectory) + args + "_mean.png";
  auto targetPath = string(LongExposureTargetDirectory) + args + "_mean.png";

  ASSERT_TRUE(compareImages(targetPath, resultPath, total_image));
}

TEST_P(LongExposureData, BrightenExposure){

  auto args = GetParam();
  std::string folder = string(LongExposureDirectory) + args;
  BrightenStacker b;
  int img_count = 0;
  for (const auto & entry : filesystem::directory_iterator(folder))
  {
      Mat img = loadStdImage(entry.path().c_str());
      b.AddToStack(img);
      img_count ++;
  }   

  auto total_image = b.GetCurrentBlend();
        
  auto resultPath = string(LongExposureTestResultsDirectory) + args + "_brighten.png";
  auto targetPath = string(LongExposureTargetDirectory) + args + "_brighten.png";

  ASSERT_TRUE(compareImages(targetPath, resultPath, total_image));
}

TEST(LongExposureExceptions, ResizedImageBrighten)
{

  auto initialPhoto = string(LongExposureDirectory) + "exception_photos/initial_photo.png";
  auto resizedPhoto = string(LongExposureDirectory) + "exception_photos/photo_resized.png";

  BrightenStacker b;

  Mat imgInit = loadStdImage(initialPhoto);
  Mat imgResized = loadStdImage(resizedPhoto);

  b.AddToStack(imgInit);
  
  EXPECT_THROW({b.AddToStack(imgResized);}, StackResizedException);

}

TEST(LongExposureExceptions, ReformattedImageBrighten)
{
  auto initialPhoto = string(LongExposureDirectory) + "exception_photos/initial_photo.png";

  BrightenStacker b;

  Mat imgInit = loadStdImage(initialPhoto);
  Mat imgReformatted = loadStdImage(initialPhoto);
  imgReformatted.convertTo(imgReformatted, CV_16SC2);

  b.AddToStack(imgInit);
  
  EXPECT_THROW({b.AddToStack(imgReformatted);}, TypeMismatchException);
}

TEST(LongExposureExceptions, ResizedImageAvg)
{

  auto initialPhoto = string(LongExposureDirectory) + "exception_photos/initial_photo.png";
  auto resizedPhoto = string(LongExposureDirectory) + "exception_photos/photo_resized.png";

  MeanStacker m;

  Mat imgInit = loadStdImage(initialPhoto);
  Mat imgResized = loadStdImage(resizedPhoto);

  m.AddToStack(imgInit);
  
  EXPECT_THROW({m.AddToStack(imgResized);}, StackResizedException);

}

TEST(LongExposureExceptions, ReformattedImageAvg)
{
  auto initialPhoto = string(LongExposureDirectory) + "exception_photos/initial_photo.png";

  MeanStacker m;

  Mat imgInit = loadStdImage(initialPhoto);
  Mat imgReformatted = loadStdImage(initialPhoto);
  imgReformatted.convertTo(imgReformatted, CV_16SC2);

  m.AddToStack(imgInit);
  
  EXPECT_THROW({m.AddToStack(imgReformatted);}, TypeMismatchException);
}