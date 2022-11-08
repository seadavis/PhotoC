#include "processing.h"
#include <eigen3/Eigen/Sparse>
#include <iostream>
#include <algorithm>
#include <vector>
#include <opencv2/imgcodecs.hpp>

using namespace cv;
using namespace std;
using namespace Eigen;

typedef SparseMatrix<float> SpMat;
typedef Eigen::Triplet<float> MatTriplet;

static string type2str(int type) {
  string r;

  uchar depth = type & CV_MAT_DEPTH_MASK;
  uchar chans = 1 + (type >> CV_CN_SHIFT);

  switch ( depth ) {
    case CV_8U:  r = "8U"; break;
    case CV_8S:  r = "8S"; break;
    case CV_16U: r = "16U"; break;
    case CV_16S: r = "16S"; break;
    case CV_32S: r = "32S"; break;
    case CV_32F: r = "32F"; break;
    case CV_64F: r = "64F"; break;
    default:     r = "User"; break;
  }

  r += "C";
  r += (chans+'0');

  return r;
}

static bool is_mask_pixel(Mat& m, unsigned int x, unsigned int y)
{   
    auto size = m.size();
    if(x < 0 || y < 0) return false;

    if(x >= size.width || y >= size.height) return false;

    return m.at<Vec4b>(Point(x, y))[3] == 0;
}

static unsigned int flatten(Mat& m, unsigned int x, unsigned int y) 
{
	return  m.size().width * y + x;
}


static float clamp(float x) {
	if (x > 1.0) {
		return 1.0;
	}
	else if (x < 0.0) {
		return 0.0;
	}
	else {
		return x;
	}
}

static SpMat form_matrix(Mat& m, map<unsigned int, unsigned int>& variable_map)
{
    auto size = m.size();

    // building the variable numbers
    // each pixel is associated with a variable number
    unsigned int variable_number = 0;
    for(unsigned int y = 0; y < size.height; y++)
    {
        for(unsigned int x = 0; x < size.width; x++)
        {
            if(is_mask_pixel(m, x, y))
            {
                variable_map[flatten(m, x, y)] = variable_number;
                variable_number++;
            }
        }
    }

    const unsigned int num_unknowns = (unsigned int)variable_map.size();
    unsigned int row = 0;

    // we assign the matrix into a triplet so what we can put it into a 
    // sparse matrix
    vector<MatTriplet> mt;

    for(unsigned int y = 0; y < size.height; y++)
    {
        for(unsigned int x = 0; x < size.width; x++)
        {
            if(is_mask_pixel(m, x, y)){
                mt.push_back(MatTriplet(row, variable_map[flatten(m, x, y)], 4));

                if(is_mask_pixel(m, x, y -1 )){
                    mt.push_back(MatTriplet(row, variable_map[flatten(m, x, y - 1)], -1));
                }

                if(is_mask_pixel(m, x + 1, y )){
                    mt.push_back(MatTriplet(row, variable_map[flatten(m, x + 1, y)], -1));
                }

                if(is_mask_pixel(m, x , y  + 1)){
                    mt.push_back(MatTriplet(row, variable_map[flatten(m, x, y + 1)], -1));
                }

                if(is_mask_pixel(m, x - 1, y)){
                    mt.push_back(MatTriplet(row, variable_map[flatten(m, x - 1, y)], -1));
                }

                row++;
            }   
        }
    }

    SpMat mat(num_unknowns, num_unknowns);
    mat.setFromTriplets(mt.begin(), mt.end());
    return mat;
}

/**
 * Forms a b vector for the given matrix 
 * and the given channel number.
 * Channel referring to BGR (due to OpenCV implementation quirk).
 * 
 * @param source_image the image with the object that is being pasted into target_image
 * @param target_image the image that is being pasted INTO.
 * @param mx - the position of source into target, relative to the upper left hand corner of both
 * @param my - the position of source into target, relative to the upper left hand corner of both
 * @param channel_number the color channel we are solving for.
 **/
static VectorXf form_target_sln(Mat& source_image, Mat& target_image, unsigned int mx, unsigned int my, unsigned int channel_number, unsigned int num_unknowns)
{
    auto size = source_image.size();
    int h = target_image.size().height;
    int w = target_image.size().width;

    unsigned int row = 0;

    VectorXf b(num_unknowns);

    for(unsigned int y = 0; y < size.height; y++)
    {
        for(unsigned int x = 0; x < size.width; x++)
        {
            if(is_mask_pixel(source_image, x, y))
            {
                float pixel = source_image.at<Vec3b>(y,x)[channel_number];
                
                float grad = 
                    pixel -  source_image.at<Vec3f>(y - 1,x)[channel_number] + 
                    pixel -  source_image.at<Vec3f>( y,x - 1)[channel_number]+ 
                    pixel -  source_image.at<Vec3f>(y + 1, x)[channel_number] + 
                    pixel -  source_image.at<Vec3f>( y,x + 1)[channel_number];


                b[row] = grad;

                if(!is_mask_pixel(source_image, x, y - 1)){
                    b[row] += target_image.at<Vec3f>(y + my - 1, x + mx)[channel_number];
                }

                if(!is_mask_pixel(source_image, x - 1, y)){
                    b[row] += target_image.at<Vec3f>(y + my, x - 1 + mx)[channel_number];
                }

                if(!is_mask_pixel(source_image, x, y + 1)){
                    b[row] += target_image.at<Vec3f>(y + 1 + my,  x + mx)[channel_number];
                }

                if(!is_mask_pixel(source_image, x + 1, y )){
                    b[row] += target_image.at<Vec3f>( y + my, x + mx + 1)[channel_number];
                }
                row++;
            }
        }
    }

    return b;

}


Mat processing::composite(Mat src, Mat tgt, unsigned int mx, unsigned int my)
{
    
    cout << "Src Type: " << type2str(src.type()) << " Tgt Type: " << type2str(tgt.type()) << "\n";

    Mat output_img = tgt.clone();
    //map<unsigned int, unsigned int> variable_map;

    //const int pre_size = variable_map.size();
    //auto src_sz = src.size();
    //auto matrix = form_matrix(src, variable_map);
    //SimplicialCholesky<SpMat> solver;
    //solver.compute(matrix);
    //const int post_size = variable_map.size();

    //VectorXf solution_channels[3];
    // solve for three seperate channelsheight
    //for(int i = 0; i < 3; i++)
    //{   
    //    auto b = form_target_sln(src, tgt, mx, my, i, (unsigned int)variable_map.size());
    //   solution_channels[i] = solver.solve(b);
    //}


    // put the solved channels into the output matrix
    for(int y = 0; y < src.rows; y++)
    {
        for(int x = 0; x < src.cols; x++)
        {
            //unsigned int variable_number = variable_map[flatten(src, x, y)];

            //const float b_raw = solution_channels[0][variable_number];
            //const float g_raw = solution_channels[1][variable_number];
            //const float r_raw = solution_channels[2][variable_number]; 

            if(!is_mask_pixel(src, x, y))
            {
                auto v = src.at<Vec3b>(Point(x, y));
                auto b_raw = v[0];
                auto g_raw = v[1];
                auto r_raw = v[2];

                auto p = Point(x + mx, y + my);
                
                output_img.at<Vec3b>(p)[0] = b_raw;
                output_img.at<Vec3b>(p)[1] = g_raw; 
                output_img.at<Vec3b>(p)[2] = r_raw;   
            }
            
            
        }
    }

    cout << "Out Type: " << type2str(output_img.type()) << " Src Type: " << type2str(src.type()) << " Tgt Type: " << type2str(tgt.type()) << "\n";
    return output_img;
}



