#include "processing.h"
#include <iostream>
#include <algorithm>

using namespace cv;

Mat processing::composite(Mat src, Mat tgt, double ax, double ay)
{
    Eigen::Matrix3f A = Eigen::Matrix3f::Zero();
    Eigen::Vector3f b;


   for(int i = 0; i < src.cols; i++){
       for(int j = 0; j < src.rows; j++){
           A(i, j) = src.at<double>(i, j);
       }
   }

   b << 3, 3, 4;
   std::cout << "Here is the matrix A:\n" << A << std::endl;
   std::cout << "Here is the vector b:\n" << b << std::endl;
   Eigen::Vector3f x = A.colPivHouseholderQr().solve(b);
   std::cout << "The solution is:\n" << x << std::endl;

   /* 
    Technically a memory leak
    but good enough to get going. The problem here is that Eigen only allocates things on the stack
    I think this actually won't be a problem with the real algorithm since we'll just straight up copy the numbers
    */
   double* data = new double[x.size()];
   std::copy(x.data(), x.data() + x.size(), data);

   return Mat(3, 1, CV_8UC1, data);
}