#include <gtest/gtest.h>
#include "MeanStacker.h"
#include <opencv2/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <filesystem>


TEST(LongExposures, ShortExposure){

  std::string path = "./src/blending/tests/long_exposure/5Photos_10Seconds";
  Mat total_image = Mat(cv::Size(4496, 3000), CV_8UC3, Scalar(0)); 
  for (const auto & entry : filesystem::directory_iterator(path))
  {
      Mat img = imread(entry.path().c_str());

      // we can't just add as that makes everything much too bright
      // we'll have to come up with some more intelligent stacking algorithms.
      // and they all need to be wicked fast.
      // which will then turn this into a problem.
      total_image = total_image + img*0.25;
  }

  total_image = total_image;
        
  imwrite("./src/blending/tests/long_exposure_tests/5Photos_10Seconds/test.png", total_image);
}