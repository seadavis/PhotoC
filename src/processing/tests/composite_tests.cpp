#include <gtest/gtest.h>
#include "processing.h"
#include <iostream>
#include <opencv2/core.hpp>

#include <opencv2/imgproc/imgproc.hpp>
#include <vector>

using namespace std;
using namespace processing;

class Processing :
    public testing::TestWithParam<tuple<string, string, int, int>> {
};

INSTANTIATE_TEST_SUITE_P(CompositeTests,
                         Processing,
                         testing::Values(

                          make_tuple("eagle", "lake", 1300, 1300), 
                          make_tuple("eagle", "beach",1300, 1300),
                          make_tuple("eagle", "gothenburg",1300, 1300), 
                          make_tuple("eagle", "liberty",1300, 1300),
                          make_tuple("eagle", "library",1300, 1300),

                          make_tuple("balloon", "lake",1300, 1300), 
                          make_tuple("balloon", "beach",1300, 1300),
                          make_tuple("balloon", "gothenburg",1300, 1300), 
                          make_tuple("balloon", "liberty",1300, 1300),
                          make_tuple("balloon", "library",150, 150),

                          make_tuple("kitten", "lake",1300, 1300), 
                          make_tuple("kitten", "beach",1300, 1300),
                          make_tuple("kitten", "gothenburg",1300, 1300), 
                          make_tuple("kitten", "liberty",1300, 1300),
                          make_tuple("kitten", "library",1300, 1300),
                          make_tuple("kitten", "collosseum",450, 450),

                          make_tuple("moon", "lake",1300, 1300), 
                          make_tuple("moon", "beach",1300, 1300),
                          make_tuple("moon", "gothenburg",1300, 1300), 
                          make_tuple("moon", "liberty",1300, 1300),
                          make_tuple("moon", "library",1300, 1300),

                          make_tuple("penguin", "lake",1300, 1300), 
                          make_tuple("penguin", "beach",1300, 1300),
                          make_tuple("penguin", "gothenburg",1300, 1300), 
                          make_tuple("penguin", "liberty",1300, 1300),
                          make_tuple("penguin", "library",1300, 1300),

                          make_tuple("watereagle", "lake",1300, 1300), 
                          make_tuple("watereagle", "beach",1300, 1300),
                          make_tuple("watereagle", "gothenburg",1300, 1300), 
                          make_tuple("watereagle", "liberty",1300, 1300),
                          make_tuple("watereagle", "library",1300, 1300)
                          
                        ));

TEST_P(Processing, BasicComposite) {

  auto args = GetParam();

  
  auto backgroundImagePath = "./src/processing/tests/target_images/" + get<1>(args) + ".png";
  auto backgroundImageRGB = imread(backgroundImagePath, CV_LOAD_IMAGE_UNCHANGED);

  Mat backgroundImage;
  cvtColor(backgroundImageRGB, backgroundImage, CV_BGR2BGRA);

  auto mask = "./src/processing/tests/masks/" + get<0>(args) + ".png";
  auto original = "./src/processing/tests/original_source_images/" + get<0>(args) + ".png";

  auto canvas = CompositeCanvas(get<3>(args), get<2>(args));
  canvas.setBackground(backgroundImage);
  canvas.setComposite(mask, original);

  Mat result = canvas.currentImg();

  auto fileName =  get<0>(args) + "_" + get<1>(args) + "_" + to_string(get<2>(args)) + "_" + to_string(get<3>(args)) + ".png";

  imwrite( "./src/processing/tests/test_composites/" + fileName, result);
  /*Mat expectedMat = imread("./src/processing/tests/target_composites/" + fileName);

  bool const equal = std::equal(result.begin<uchar>(), result.end<uchar>(), expectedMat.begin<uchar>());
  ASSERT_TRUE(equal);*/
}