#include <gtest/gtest.h>
#include "processing.h"
#include <iostream>

using namespace std;

// Demonstrate some basic assertions.
TEST(Processing, BasicComposite) {

  double data[9] = {1.0,2.0,3.0,4.0,5.0,6.0, 7.0,8.0,10.0};
  Mat m1 = Mat(3,3, CV_8UC1,&data);

  Mat m2 = Mat(0,0, CV_8UC1);
  Mat result = processing::composite(m1, m2, 0.0, 0.0);

  double expected_data[3] = {-2.0, 1.0, 1.0};
  Mat expectedMat = Mat(3,1, CV_8UC1, &expected_data);

  bool const equal = std::equal(result.begin<uchar>(), result.end<uchar>(), expectedMat.begin<uchar>());
  ASSERT_TRUE(equal);
}