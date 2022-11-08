#include <gtest/gtest.h>
#include "processing.h"
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;


TEST(Processing, BasicComposite) {


  // Read in m1
  // Read in m2 (transparent) pixels
  // place m2 in centre of m1
  // test the resulting image (for now just place into folder)
  // then set up regressions
  Mat m1 = imread("./src/processing/tests/target_images/lake.png", IMREAD_UNCHANGED);
  Mat m2 = imread("./src/processing/tests/source_images/eagle.png", IMREAD_UNCHANGED);

  Mat tgt;
  cvtColor(m1, tgt, CV_BGR2BGRA);

  unsigned int tgt_height = m1.size().height;
  unsigned int tgt_width = m1.size().width;

  unsigned int tgt_cy = tgt_height/2;
  unsigned int tgt_cx = tgt_width/2;

  unsigned int src_cy = m2.size().height/2;
  unsigned int src_cx = m2.size().width/2;

  Mat result = processing::composite(m2, tgt, tgt_cx - src_cx, tgt_cy - src_cy );

  Mat result_out;

  imwrite("./src/processing/tests/target_composites/eagle_lake.png", result);

  //bool const equal = std::equal(result.begin<uchar>(), result.end<uchar>(), expectedMat.begin<uchar>());
  ASSERT_TRUE(false);
}