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
 * Convers the given open cv matrix to 
 * 3 eigen matrices green, blue and red
 **/
static vector<Eigen::MatrixXf> cv_to_eigen_channels(Mat m)
{
    auto const size = m.size();
    vector<Eigen::MatrixXf> mat_list(3);

    mat_list.push_back(Eigen::MatrixXf(size.width, size.height));
    mat_list.push_back(Eigen::MatrixXf(size.width, size.height));
    mat_list.push_back(Eigen::MatrixXf(size.width, size.height));
    
    for(unsigned int y = 0; y < size.height; y++)
    {
        for(unsigned x = 0; x < size.width; x++)
        {
            auto v = m.at<Vec4b>(Point(x, y));

            mat_list[0](x, y) = (float)v[0]/255.0f;
            mat_list[1](x, y) = (float)v[1]/255.0f;
            mat_list[2](x, y) = (float)v[2]/255.0f;
        }
    }

    return mat_list;
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
static vector<VectorXf> form_target_slns(Mat& source_image, Mat& target_image, unsigned int mx, unsigned int my, unsigned int num_unknowns)
{
    auto size = source_image.size();
    int h = target_image.size().height;
    int w = target_image.size().width;

    unsigned int row = 0;

    auto source_channels = cv_to_eigen_channels(source_image);
    auto target_channels = cv_to_eigen_channels(target_image);

    vector<VectorXf> solution_channels(3);

    for(int channel_number = 0; channel_number < 3; channel_number++)
    {
        VectorXf b(num_unknowns);

        for(unsigned int y = 0; y < size.height; y++)
        {
            for(unsigned int x = 0; x < size.width; x++)
            {
                if(is_mask_pixel(source_image, x, y))
                {
                    float pixel = source_channels[channel_number](x, y);
                    
                    float grad = 
                        pixel -  source_channels[channel_number](x,y - 1) + 
                        pixel -  source_channels[channel_number](x-1, y)+ 
                        pixel -  source_channels[channel_number](x, y+1) + 
                        pixel - source_channels[channel_number](x+1, y);


                    b[row] = grad;

                    if(!is_mask_pixel(source_image, x, y - 1)){
                        b[row] += target_channels[channel_number](x + mx, y + my - 1);
                    }

                    if(!is_mask_pixel(source_image, x - 1, y)){
                        b[row] += target_channels[channel_number](x -1 + mx, y + my);
                    }

                    if(!is_mask_pixel(source_image, x, y + 1)){
                        b[row] += target_channels[channel_number](x + mx, y + 1 + my);
                    }

                    if(!is_mask_pixel(source_image, x + 1, y )){
                        b[row] += target_channels[channel_number](x + mx +1, y + my);
                    }
                    row++;
                }
            }
        }
    }

    return solution_channels;

}


Mat processing::composite(Mat src, Mat tgt, unsigned int mx, unsigned int my)
{
    
    Mat output_img = tgt.clone();
    map<unsigned int, unsigned int> variable_map;

    auto src_sz = src.size();

    auto matrix = form_matrix(src, variable_map);
    SimplicialCholesky<SpMat> solver;
    solver.compute(matrix);

    vector<VectorXf> solution_channels = form_target_slns(src, tgt, mx, my, (unsigned int)variable_map.size());
    
    // put the solved channels into the output matrix
    for(int y = 0; y < src.rows; y++)
    {
        for(int x = 0; x < src.cols; x++)
        {
           
            if(!is_mask_pixel(src, x, y))
            {
                unsigned int variable_number = variable_map[flatten(src, x, y)];
                const float b_raw = solution_channels[0][variable_number];
                const float g_raw = solution_channels[1][variable_number];
                const float r_raw = solution_channels[2][variable_number]; 


                // here we clamp between 0 and 1.0 and then multiple to 255
                auto b = (int)(clamp(b_raw)*255.0);
                auto g = (int)(clamp(g_raw)*255.0);
                auto r = (int)(clamp(r_raw)*255.0);

                auto p = Point(x + mx, y + my);
                
                output_img.at<Vec4b>(p)[0] = b_raw;
                output_img.at<Vec4b>(p)[1] = g_raw; 
                output_img.at<Vec4b>(p)[2] = r_raw;   
            }
            
            
        }
    }

    return output_img;
}



