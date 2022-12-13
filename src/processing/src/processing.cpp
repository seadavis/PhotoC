#include "processing.h"
#include <eigen3/Eigen/Sparse>
#include <algorithm>
#include <opencv2/imgcodecs.hpp>
#include <vector>

using namespace std;
using namespace Eigen;
using namespace processing;

typedef SparseMatrix<float> SpMat;
typedef Eigen::Triplet<float> MatTriplet;

constexpr float GAMMA = 2.2f;

static bool is_mask_pixel(Mat& m, unsigned int x, unsigned int y)
{   
    auto size = m.size();
    if(x < 0 || y < 0) return false;

    if(x >= size.width || y >= size.height) return false;

    return m.at<Vec4b>(Point(x, y))[3] > 0;
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

static inline float in_pixel(float pixel)
{
    return pow(pixel/255.0f, 1.0f/GAMMA);
}

static inline float out_pixel(float pixel)
{
    return pow(clamp(pixel), GAMMA)*255.0f;
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

    mat_list[0] = Eigen::MatrixXf(size.height, size.width);
    mat_list[1] = Eigen::MatrixXf(size.height, size.width);
    mat_list[2] = Eigen::MatrixXf(size.height, size.width);

    for(unsigned int y = 0; y < size.height; y++)
    {
        for(unsigned int x = 0; x < size.width; x++)
        {
            auto v = m.at<Vec4b>(Point(x, y));
            mat_list[0](y, x) = in_pixel((float)v[0]);
            mat_list[1](y, x) = in_pixel((float)v[1]);
            mat_list[2](y, x) = in_pixel((float)v[2]);
        }
    }
    
    return mat_list;
}

/**
 * Forms a b vector for the given matrix 
 * and the given channel number.
 * Channel referring to BGR (due to OpenCV implementation quirk).
 * 
 * @param mask_image the portion of the image we want to paste
 * @param source_image the image with the object that is being pasted into target_image
 * @param target_image the image that is being pasted INTO.
 * @param mx - the position of source into target, relative to the upper left hand corner of both
 * @param my - the position of source into target, relative to the upper left hand corner of both
 * @param channel_number the color channel we are solving for.
 **/
static vector<VectorXf> form_target_slns(Mat& mask_image, Mat& source_image, Mat& target_image, unsigned int mx, unsigned int my, unsigned int num_unknowns, Eigen::SimplicialCholesky<SpMat>& solver)
{
    auto size = source_image.size();
    int h = target_image.size().height;
    int w = target_image.size().width;

    auto source_channels = cv_to_eigen_channels(source_image);
    auto target_channels = cv_to_eigen_channels(target_image);

    vector<VectorXf> solution_channels(3);

    for(int channel_number = 0; channel_number < 3; channel_number++)
    {
        unsigned int row = 0;
        VectorXf b(num_unknowns);

        for(unsigned int y = 0; y < size.height; y++)
        {
            for(unsigned int x = 0; x < size.width; x++)
            {
                if(is_mask_pixel(mask_image, x, y))
                {
                   
                    float pixel = source_channels[channel_number](y, x);
                
                    float grad = 
                        pixel -  source_channels[channel_number](y - 1,x) + 
                        pixel -  source_channels[channel_number](y,x-1)+ 
                        pixel -  source_channels[channel_number](y+1,x) + 
                        pixel - source_channels[channel_number](y,x+1);

                    b[row] = grad;

                    if(!is_mask_pixel(mask_image, x, y - 1)){
                        b[row] += target_channels[channel_number](y + my - 1, x + mx);
                    }

                    if(!is_mask_pixel(mask_image, x - 1, y)){
                        b[row] += target_channels[channel_number](y + my, x - 1 + mx);
                    }

                    if(!is_mask_pixel(mask_image, x, y + 1)){
                        b[row] += target_channels[channel_number](y + 1 + my, x + mx);
                    }

                    if(!is_mask_pixel(mask_image, x + 1, y )){
                        b[row] += target_channels[channel_number](y + my, x + mx +1);
                    }
                    row++;
                }
            }
        }

        solution_channels[channel_number] = solver.solve(b);
    }

    return solution_channels;
}


Mat processing::composite(Mat mask, Mat src, Mat tgt, unsigned int mx, unsigned int my)
{
    
    Mat output_img = tgt.clone();
    map<unsigned int, unsigned int> variable_map;

    auto src_sz = src.size();

    auto matrix = form_matrix(mask, variable_map);
    SimplicialCholesky<SpMat> solver;
    solver.compute(matrix);

    vector<VectorXf> solution_channels = form_target_slns(mask, src, tgt, mx, my, (unsigned int)variable_map.size(), solver);
    
    // put the solved channels into the output matrix
    for(int y = 0; y < src.rows; y++)
    {
        for(int x = 0; x < src.cols; x++)
        {
           
            if(is_mask_pixel(mask, x, y))
            {
                unsigned int variable_number = variable_map[flatten(mask, x, y)];
                const float b_raw = solution_channels[0][variable_number];
                const float g_raw = solution_channels[1][variable_number];
                const float r_raw = solution_channels[2][variable_number]; 

                auto b = (int)(out_pixel(b_raw));
                auto g = (int)(out_pixel(g_raw));
                auto r = (int)(out_pixel(r_raw));

                auto p = Point(x + mx, y + my);
                
                output_img.at<Vec4b>(p)[0] = b;
                output_img.at<Vec4b>(p)[1] = g; 
                output_img.at<Vec4b>(p)[2] = r; 
            }
            
            
        }
    }

    return output_img;
}

Mat processing::bounding_rectangle(Mat& src)
{
    Mat src_gray;
    cvtColor(src, src_gray, CV_BGRA2GRAY);

    blur(src_gray, src_gray, Size(3, 3));

    Mat canny_output;

    double threshold = 0.05;
    Canny(src_gray, canny_output, threshold, threshold*2);

    vector<vector<Point>> contours;
  

    findContours( canny_output, contours, RETR_TREE, CHAIN_APPROX_SIMPLE );

    Rect bounding_rectangle;

    vector<vector<Point>> countours_poly(contours.size());
    approxPolyDP(contours[0], countours_poly[0], 3, true);
    bounding_rectangle = boundingRect(countours_poly[0]);

    rectangle(src, bounding_rectangle, Scalar(0, 255, 0, 255));
    return src;
}

Mat processing::make_canvas(Mat src, int width, int height, int width_padding, int height_padding)
{
    int total_width = width + width_padding;
    int total_height = height + height_padding;

    Mat canvas(total_height, total_width, CV_8UC3, cv::Scalar(0, 0, 0));
    Mat sized_src;

    if(src.size().width < width && src.size().height < height)
    {
        sized_src = src;
    }
    else if(src.size().width >= width || src.size().height >= height)
    {
        auto width_factor = (float)width/(float)src.size().width;
        auto height_factor = (float)height/(float)src.size().height;

        auto greatest_factor = width_factor < height_factor ? width_factor : height_factor;

        auto new_width = greatest_factor*src.size().width;
        auto new_height = greatest_factor*src.size().height;

        resize(src, sized_src, Size(new_width, new_height), INTER_AREA);
    }

    auto size = sized_src.size();
   
    int mx = total_width/2 - size.width/2;
    int my = total_height/2 - size.height/2;

    for(int y = 0; y < sized_src.rows; y++)
    {
        for(int x = 0; x < sized_src.cols; x++)
        {
            auto v = sized_src.at<Vec4b>(Point(x, y));
            auto b = v[0];
            auto g = v[1];
            auto r = v[2];

            auto p = Point(x + mx, y + my);
            canvas.at<Vec3b>(p)[0] = b;
            canvas.at<Vec3b>(p)[1] = g; 
            canvas.at<Vec3b>(p)[2] = r; 
        }
    }

    return canvas;
}

CompositeCanvas::CompositeCanvas(int height, int width)
{
    this->height = height;
    this->width = width;
}

void CompositeCanvas::setBackground(const string& imagePath)
{
    Mat img = imread(imagePath, CV_LOAD_IMAGE_UNCHANGED);
    Mat tgt;
    cvtColor(img, tgt, CV_BGR2BGRA);
    backgroundImage = unique_ptr<Mat>(new Mat(tgt));
}

void CompositeCanvas::setComposite(const string& maskImgPath, const string& originalImage)
{

}


 Mat CompositeCanvas::currentImg()
 {
    Mat m;
    m.setTo(Scalar(0, 0, 0));
    return m;
 }
