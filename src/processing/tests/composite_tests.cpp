#include <gtest/gtest.h>
#include "processing.h"
#include <iostream>
#include <opencv2/core.hpp>

#include <opencv2/imgproc/imgproc.hpp>
#include <vector>

using namespace std;
using namespace processing;

class Processing :
    public testing::TestWithParam<tuple<string, string>> {
};

INSTANTIATE_TEST_SUITE_P(CompositeTests,
                         Processing,
                         testing::Values(

                          make_tuple("eagle", "lake"), 
                          make_tuple("eagle", "beach"),
                          make_tuple("eagle", "gothenburg"), 
                          make_tuple("eagle", "liberty"),
                          make_tuple("eagle", "library"),

                          make_tuple("balloon", "lake"), 
                          make_tuple("balloon", "beach"),
                          make_tuple("balloon", "gothenburg"), 
                          make_tuple("balloon", "liberty"),
                          make_tuple("balloon", "library"),

                          make_tuple("kitten", "lake"), 
                          make_tuple("kitten", "beach"),
                          make_tuple("kitten", "gothenburg"), 
                          make_tuple("kitten", "liberty"),
                          make_tuple("kitten", "library"),

                          make_tuple("moon", "lake"), 
                          make_tuple("moon", "beach"),
                          make_tuple("moon", "gothenburg"), 
                          make_tuple("moon", "liberty"),
                          make_tuple("moon", "library"),

                          make_tuple("penguin", "lake"), 
                          make_tuple("penguin", "beach"),
                          make_tuple("penguin", "gothenburg"), 
                          make_tuple("penguin", "liberty"),
                          make_tuple("penguin", "library"),

                          make_tuple("watereagle", "lake"), 
                          make_tuple("watereagle", "beach"),
                          make_tuple("watereagle", "gothenburg"), 
                          make_tuple("watereagle", "liberty"),
                          make_tuple("watereagle", "library")
                          
                        ));

TEST_P(Processing, BasicComposite) {

  auto args = GetParam();

  
  auto backgroundImagePath = "./src/processing/tests/target_images/" + get<1>(args) + ".png";
  auto backgroundImageRGB = imread(backgroundImagePath, CV_LOAD_IMAGE_UNCHANGED);

  Mat backgroundImage;
  cvtColor(backgroundImageRGB, backgroundImage, CV_BGR2BGRA);

  auto mask = "./src/processing/tests/masks/" + get<0>(args) + ".png";
  auto original = "./src/processing/tests/original_source_images/" + get<0>(args) + ".png";

  auto canvas = CompositeCanvas(2500, 2500);
  canvas.setBackground(backgroundImage);
  canvas.setComposite(mask, original);

  Mat result = canvas.currentImg();
  imwrite( "./src/processing/tests/test_composites/" + get<0>(args) + "_" + get<1>(args) + ".png", result);
  Mat expectedMat = imread("./src/processing/tests/target_composites/" + get<0>(args) + "_" + get<1>(args) + ".png");

  bool const equal = std::equal(result.begin<uchar>(), result.end<uchar>(), expectedMat.begin<uchar>());
  ASSERT_TRUE(equal);
}