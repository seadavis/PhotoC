#include "processing.h"
#include <eigen3/Eigen/Sparse>
#include <algorithm>
#include <opencv2/imgcodecs.hpp>
#include <vector>
#include <cmath>

using namespace std;
using namespace Eigen;
using namespace processing;

typedef SparseMatrix<float> SpMat;
typedef Eigen::Triplet<float> MatTriplet;

constexpr float GAMMA = 2.2f;
constexpr int Radius = 5;

const Scalar SelectionColor = Scalar(0, 255, 0, 255);

static bool is_mask_pixel(Mat& m, unsigned int x, unsigned int y)
{   
    auto size = m.size();
    if(x < 0 || y < 0) return false;

    if(x >= size.width || y >= size.height) return false;

    return m.at<Vec4b>(cv::Point(x, y))[3] > 0;
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
            auto v = m.at<Vec4b>(cv::Point(x, y));
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

 /**
 * Pastes the mask image into the target image,
 * adjusting the color using the source, which
 * is where the mask came from.
 * 
 * Assumes all images are in a BGRA format.
 * dx and dy are specified such that mask has at least one 
 * pixel between itself and the target.
 * 
 * @param mask the portion of the image we want to  
 * any fully transparent pixels will not be pasted into target
 * 
 * @param src the original image the mask came from
 * 
 * @param target the "target" we want to put the image into
 * 
 * @param dx specifies in pixel space how far horizontally
 * we want to start pasting the image, Left most pixel is 0.
 * 
 * @param dy specifies in pixel space how far vertically
 * we want to start pasting the image, upper most pixel is 0.
 */
static Mat composite(Mat mask, Mat src, Mat tgt, unsigned int mx, unsigned int my)
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

static Mat naive_composite(Mat mask, Mat src, Mat tgt,  unsigned int mx, unsigned int my)
{
    Mat output_img = tgt.clone();
    
    // put the solved channels into the output matrix
    for(int y = 0; y < src.rows; y++)
    {
        for(int x = 0; x < src.cols; x++)
        {
            if(is_mask_pixel(mask, x, y))
            {
                auto v = mask.at<Vec4b>(Point(x, y));
                auto b = v[0];
                auto g = v[1];
                auto r = v[2];

                auto p = Point(x + mx, y + my);
                
                output_img.at<Vec4b>(p)[0] = b;
                output_img.at<Vec4b>(p)[1] = g; 
                output_img.at<Vec4b>(p)[2] = r; 
            }
        }
    }

    return output_img;
}

static Mat size_to_fit(Mat src, int width, int height)
{
    Mat sized_src;
    if(src.size().width <= width && src.size().height <= height)
    {
        sized_src = src;
    }
    else if(src.size().width > width || src.size().height >=height)
    {
        auto width_factor = (float)width/(float)src.size().width;
        auto height_factor = (float)height/(float)src.size().height;

        auto greatest_factor = width_factor < height_factor ? width_factor : height_factor;

        auto new_width = greatest_factor*src.size().width;
        auto new_height = greatest_factor*src.size().height;

        resize(src, sized_src, Size(new_width, new_height), INTER_AREA);
    }

    return sized_src;
}

static Mat fill_in_canvas(Mat src, int width, int height)
{
    
    Mat canvas(Size(width, height), CV_8UC3, cv::Scalar(0, 0, 0));
   
    auto size = src.size();
    int mx = width/2 - size.width/2;
    int my = height/2 - size.height/2;

    for(int y = 0; y < src.rows; y++)
    {
        for(int x = 0; x < src.cols; x++)
        {
            auto v = src.at<Vec4b>(Point(x, y));
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

static ImageBorder translate_border(ImageBorder b, int x, int y)
{
    return ImageBorder(b.width(), b.height(), Point(b.tl().x + x, b.tl().y + y));
}

static bool is_point_within_circle(Point p, Point center, int radius)
{
    return pow(p.x - center.x, 2) + (p.y - center.y, 2) < pow(radius, 2);
}

int ImageBorder::width()
{
    return r.size().width;
}

int ImageBorder::height()
{
    return r.size().height;
}

Point ImageBorder::tl()
{
    return r.tl();
}

Point ImageBorder::br()
{
    return r.br();
}

ImageBorder::ImageBorder(int width, int height, Point tl)
{
    r = Rect(tl.x, tl.y, width, height);
    tl_circle = Circle(Radius, r.tl());
    br_circle = Circle(Radius, r.br());
}

void ImageBorder::draw(Mat m)
{
    rectangle(m, r, SelectionColor);
    tl_circle.draw(m);
    br_circle.draw(m);
}

ObjectType ImageBorder::hit(Point p)
{
    auto top_left = tl();
    if(is_point_within_circle(p, top_left, Radius))
    {
        return ObjectType::SizeCircle;
    }

    auto bottom_right = br();
    if(is_point_within_circle(p, bottom_right, Radius))
    {
        return ObjectType::SizeCircle;
    }

    int x_min = tl().x;
    int y_min = tl().y;

    int x_max = x_min + width();
    int y_max = y_min + height();

    if(p.x >= x_min && p.x <= x_max && p.y >= y_min && p.y <= y_max)
        return ObjectType::Image;

    return ObjectType::None;
}

void Circle::draw(Mat m)
{
    circle(m, center, radius, SelectionColor, FILLED, LINE_8);
}

ImageBorder CompositeCanvas::translate_to_canvas_coordindates(const ImageBorder& b)
{
    int x_adjustment = backgroundImage != nullptr ? (width - backgroundImage->size().width)/2 : 0;
    int y_adjustment = backgroundImage != nullptr ? (height - backgroundImage->size().height)/2 : 0;

    return translate_border(b, mx + x_adjustment, my + y_adjustment);
}

void CompositeCanvas::translate(int dx, int dy)
{
    if(objectSelected)
    {
        int mx_prime = mx + dx;
        int my_prime = my + dy;

        if(mx_prime < 1)
            mx = 5;

        else if(backgroundImage != nullptr && mx_prime + maskImage->size().width >= backgroundImage->size().width)
            mx =  backgroundImage->size().width - maskImage->size().width - 5;

        else
            mx = mx_prime;

        if(my_prime < 1)
            my = 5;

        else if(backgroundImage != nullptr && my_prime + maskImage->size().height >= backgroundImage->size().height)
            my = backgroundImage->size().height - maskImage->size().height - 5;
        
        else
            my = my_prime;
    }

}

Mat CompositeCanvas::loadImage(string imagePath)
{
    Mat img = imread(imagePath, CV_LOAD_IMAGE_UNCHANGED);
    Mat tgt;
    cvtColor(img, tgt, CV_BGR2BGRA);
    auto sizedTgt = size_to_fit(tgt, width, height);
    return sizedTgt;
}

CompositeCanvas::CompositeCanvas()
{
    this->height = 0;
    this->width = 0;
    this->backgroundImage = nullptr;
    this->maskImage = nullptr;
    this->originalImage = nullptr;
}

void CompositeCanvas::setSize(int width, int height)
{
    this->width = width;
    this->height = height;
    initPlacement();
}

void CompositeCanvas::setBackground(Mat backgrnd)
{
   auto sizedBackground = size_to_fit(backgrnd, width, height);
   backgroundImage = unique_ptr<Mat>(new Mat(sizedBackground));

   initPlacement();
}

void CompositeCanvas::setComposite(const string& maskImgPath, const string& originalImagePath)
{
    if(maskImgPath.length() > 0)
    {
        maskImage = unique_ptr<Mat>(new Mat(loadImage(maskImgPath)));
        border = ImageBorder(maskImage->size().width, maskImage->size().height, Point(0, 0));
    }
        
    if(originalImagePath.length() > 0)
        originalImage = unique_ptr<Mat>(new Mat(loadImage(originalImagePath)));

    initPlacement();
}

ObjectType CompositeCanvas::hit(Point p)
{
    auto placedBorder = translate_to_canvas_coordindates(border);
    auto hitType = placedBorder.hit(p);

    if(hitType == ObjectType::SizeCircle && !showBoundingRectangle)
    {
        return ObjectType::None;
    }

    return hitType;
}

void CompositeCanvas::tap(Point p)
{
    auto hitType = hit(p);
    if(hitType == ObjectType::Image)
    {
        showBoundingRectangle = true;
        objectSelected = true;
    }
    else
    {
        showBoundingRectangle = false;
        objectSelected = false;
    }
    
}

void CompositeCanvas::releaseObject()
{
    objectSelected = false;
}

bool CompositeCanvas::only_background_available()
{
    return backgroundImage != nullptr && 
            (maskImage == nullptr || originalImage == nullptr);
}

bool CompositeCanvas::only_src_available()
{
    return backgroundImage == nullptr && 
            (maskImage != nullptr && originalImage != nullptr);
}

bool CompositeCanvas::src_and_background_available()
{
    return backgroundImage != nullptr && 
            maskImage != nullptr && originalImage != nullptr;
}

void CompositeCanvas::draw_adornments(Mat canvas)
{
    if(showBoundingRectangle)
    {
        auto placedRect = translate_to_canvas_coordindates(border);
        placedRect.draw(canvas);
    }
}

void CompositeCanvas::initPlacement()
{
    if(src_and_background_available())
    {
        int src_cy = maskImage->size().height/2;
        int src_cx = maskImage->size().width/2;

        mx = backgroundImage->size().width/2 - src_cx;
        my = backgroundImage->size().height/2 - src_cy;
    }
    else if(only_src_available())
    {
        int src_cy = maskImage->size().height/2;
        int src_cx = maskImage->size().width/2;

        mx = width/2 - src_cx;
        my = height/2 - src_cy;
    }
}

 Mat CompositeCanvas::currentImg()
 {
    Mat img;
    
    if(only_background_available())
    {
        img = *backgroundImage;
    }

    else if(only_src_available())
    {
        img = *maskImage;
    }

    else if(originalImage != nullptr && maskImage != nullptr &&
         originalImage->size() != maskImage->size())
    {
        if(backgroundImage != nullptr)
            img = *backgroundImage;
    }

    else if(src_and_background_available())
    {

        Mat mask = *maskImage;
        Mat original = *originalImage;
        Mat background = *backgroundImage;

        int tgt_height = background.size().height;
        int tgt_width = background.size().width;

        if(tgt_width < mask.size().width || tgt_height < mask.size().height)
        {
            throw BackgroundResizedException(original.size().height,
                                            original.size().width,
                                            background.size().width,
                                            background.size().height,
                                            mask.size().width,
                                            mask.size().height);
        }

        if(showBoundingRectangle)
        {
            img = naive_composite(mask, original, background, mx, my);
        }
        else
        {
            img = composite(mask, original, background, mx, my);
        }
       
    }

    auto canvas = fill_in_canvas(img, width, height);
    draw_adornments(canvas);
    return canvas;
 }
