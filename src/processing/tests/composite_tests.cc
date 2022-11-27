#include <gtest/gtest.h>
#include "processing.h"
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>

using namespace std;

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

  
  // Read in m1
  // Read in m2 (transparent) pixels
  // place m2 in centre of m1
  // test the resulting image (for now just place into folder)
  // then set up regressions
  Mat m1 = imread("./src/processing/tests/target_images/" + get<1>(args) + ".png", IMREAD_UNCHANGED);
  Mat m2 = imread("./src/processing/tests/masks/" + get<0>(args) + ".png", IMREAD_UNCHANGED);
  Mat m3 = imread("./src/processing/tests/original_source_images/" + get<0>(args) + ".png", IMREAD_UNCHANGED);

  Mat tgt;
  cvtColor(m1, tgt, CV_BGR2BGRA);

  Mat src;
  cvtColor(m3, src, CV_BGR2BGRA);

  unsigned int tgt_height = m1.size().height;
  unsigned int tgt_width = m1.size().width;

  unsigned int tgt_cy = tgt_height/2;
  unsigned int tgt_cx = tgt_width/2;

  unsigned int src_cy = m2.size().height/2;
  unsigned int src_cx = m2.size().width/2;

  Mat result = processing::composite(m2, src, tgt, tgt_cx - src_cx, tgt_cy - src_cy );
  imwrite( "./src/processing/tests/test_composites/" + get<0>(args) + "_" + get<1>(args) + ".png", result);
  Mat expectedMat = imread("./src/processing/tests/target_composites/" + get<0>(args) + "_" + get<1>(args) + ".png");

  bool const equal = std::equal(result.begin<uchar>(), result.end<uchar>(), expectedMat.begin<uchar>());
  ASSERT_TRUE(equal);
}