#include "processing.h"
#include <algorithm>
#include <future>
#include <opencv2/imgcodecs.hpp>
#include <vector>
#include <chrono>

using namespace std;
using namespace processing;
using namespace common::normalization;

constexpr int Radius = 5;
constexpr int AbsMaxHeight = 450;
constexpr int AbsMaxWidth = 450;

const Scalar SelectionColor = Scalar(0, 255, 0, 255);

static bool is_mask_pixel(Mat& m, unsigned int x, unsigned int y)
{   
    auto size = m.size();
    if(x <= 0 || y <= 0) return false;

    if(x >= size.width - 1 || y >= size.height - 1) return false;

    return m.at<Vec4b>(cv::Point(x, y))[3] > 0;
}

static inline unsigned int flatten(Mat& m, unsigned int x, unsigned int y) 
{
	return  m.size().width * y + x;
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
    vector<MatTriplet> mt(size.height*size.width);

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
 * @param mask_image the portion of the image we want to paste
 * @param source_image the image with the object that is being pasted into target_image
 * @param target_image the image that is being pasted INTO.
 * @param mx - the position of source into target, relative to the upper left hand corner of both
 * @param my - the position of source into target, relative to the upper left hand corner of both
 * @param channel_number the color channel we are solving for.
 **/
static inline VectorXf form_target_slns_for_channel(Mat* mask_image, 
                                                        Mat* source_image, 
                                                        Mat* target_image, 
                                                        unsigned int mx, 
                                                        unsigned int my,
                                                        unsigned int num_unknowns, 
                                                        Solver* solver,
                                                        int channel_number)
{
    auto size = mask_image->size();
    int h = target_image->size().height;
    int w = target_image->size().width;

    unsigned int row = 0;
    VectorXf b(num_unknowns);

    for(unsigned int y = 0; y < size.height; y++)
    {
        for(unsigned int x = 0; x < size.width; x++)
        {
            if(is_mask_pixel(*mask_image, x, y))
            {
                
                float pixel =  get_normalized_matrix_channel(*source_image,  x, y, channel_number);
            
                float grad = 
                    pixel -  get_normalized_matrix_channel(*source_image,  x, y-1, channel_number) + 
                    pixel -  get_normalized_matrix_channel(*source_image,  x-1, y, channel_number) + 
                    pixel -  get_normalized_matrix_channel(*source_image,  x, y+1, channel_number) + 
                    pixel - get_normalized_matrix_channel(*source_image,  x + 1, y, channel_number);

                b[row] = grad;

                if(!is_mask_pixel(*mask_image, x, y - 1)){
                    b[row] += get_normalized_matrix_channel(*target_image,  x + mx, y + my - 1, channel_number);
                }

                if(!is_mask_pixel(*mask_image, x - 1, y)){
                    
                    b[row] += get_normalized_matrix_channel(*target_image, x - 1 + mx, y + my, channel_number);
                }

                if(!is_mask_pixel(*mask_image, x, y + 1)){                     
                    b[row] +=  get_normalized_matrix_channel(*target_image, x + mx, y + 1 + my, channel_number);
                }

                if(!is_mask_pixel(*mask_image, x + 1, y )){
                    b[row] += get_normalized_matrix_channel(*target_image, x + mx +1, y + my, channel_number);
                }
                row++;
            }
        }
    }

    return solver->solve(b);
}

static void write_slns_to_img(Mat *outputImg, 
                            Mat *src, 
                            Mat *mask, 
                            map<unsigned int, unsigned int> *variableMap, 
                            VectorXf& solution, 
                            unsigned int mx, 
                            unsigned int my, 
                            int solutionChannel)
{
    for(int y = 0; y < mask->rows; y++)
    {
        for(int x = 0; x < mask->cols; x++)
        {
           
            if(is_mask_pixel(*mask, x, y))
            {
                unsigned int variableNumber = (*variableMap)[flatten(*mask, x, y)];
                const float raw = solution[variableNumber];
                const auto pixelValue = (int)(out_pixel(raw));
                const auto p = Point(x + mx, y + my);
                
                outputImg->at<Vec4b>(p)[solutionChannel] = pixelValue;
            }
        }
    }
}

static void solve_for_channel(Mat* outputImg, 
                            Mat* src, 
                            Mat* mask, 
                            Solver* solver,
                            map<unsigned int, unsigned int>* variableMap, 
                            unsigned int mx, 
                            unsigned int my, 
                            int solutionChannel)
{
    auto v = form_target_slns_for_channel(mask, src, outputImg, mx, my, (unsigned int)variableMap->size(), solver, solutionChannel);
    write_slns_to_img(outputImg, src, mask, variableMap, v, mx, my, solutionChannel);
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
static Mat composite(Mat& mask, 
                    Mat& src, 
                    Mat& tgt, 
                    Solver& solver, 
                    map<unsigned int, unsigned int>& variableMap,  
                    unsigned int mx, 
                    unsigned int my)
{

    Mat outputImg = tgt.clone();

    auto channel1 = async(solve_for_channel, &outputImg, &src, &mask, &solver, &variableMap, mx, my, 0);
    auto channel2 = async(solve_for_channel, &outputImg,&src, &mask, &solver, &variableMap, mx, my, 1);
    auto channel3 = async(solve_for_channel, &outputImg,&src, &mask, &solver, &variableMap, mx, my, 2);

    channel1.wait();
    channel2.wait();
    channel3.wait();
    
    return outputImg;
}

static Mat naive_composite(Mat mask, Mat src, Mat tgt,  unsigned int mx, unsigned int my)
{
    Mat output_img = tgt.clone();
    
    // put the solved channels into the output matrix
    for(int y = 0; y < mask.rows; y++)
    {
        for(int x = 0; x < mask.cols; x++)
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

ImageBorder CompositeCanvas::translate_to_canvas_coordindates(ImageBorder b)
{
    int x_adjustment = backgroundImage != nullptr ? (width - backgroundImage->size().width)/2 : 0;
    int y_adjustment = backgroundImage != nullptr ? (height - backgroundImage->size().height)/2 : 0;

    Point tl = b.tl();

    auto top_left = Point(tl.x + mx + x_adjustment, tl.y + my + y_adjustment);

    return ImageBorder(maskWidth, maskHeight, top_left);
}


void CompositeCanvas::scaleSelected(int dx, int dy)
{
    if(objectSelected == ObjectType::SizeCircle)
    {
        auto deltaHeight = maskHeight + dy;
        auto deltaWidth = maskWidth + dx;
        
        int maxHeight;
        int maxWidth;

        if(backgroundImage == nullptr)
        {
            maxHeight = AbsMaxHeight;
            maxWidth = AbsMaxWidth;
        }
        else
        {
            if(backgroundImage->size().width < AbsMaxWidth)
                maxWidth = backgroundImage->size().width;
            else
                maxWidth = AbsMaxWidth;
            
            if(backgroundImage->size().height < AbsMaxHeight)
                maxHeight = backgroundImage->size().height;
            else
                maxHeight = AbsMaxHeight;
        }

        if(deltaHeight >= maxHeight)
            deltaHeight = maxHeight - 10;

        if(deltaWidth >= maxWidth)
            deltaWidth = maxWidth - 10;

        if(deltaHeight < 10)
            deltaHeight = 10;

        if(deltaWidth < 10)
            deltaWidth = 10;

        maskWidth = deltaWidth;
        maskHeight = deltaHeight; 
        alignMaskSize();
    }
}

void CompositeCanvas::translateSelected(int dx, int dy)
{
    if(objectSelected == ObjectType::Image)
    {
        int mx_prime = mx + dx;
        int my_prime = my + dy;

        if(mx_prime < 1)
            mx = 5;

        else if(backgroundImage != nullptr && mx_prime + resizedMask->size().width >= backgroundImage->size().width)
            mx =  backgroundImage->size().width - resizedMask->size().width - 5;

        else
            mx = mx_prime;

        if(my_prime < 1)
            my = 5;

        else if(backgroundImage != nullptr && my_prime + resizedMask->size().height >= backgroundImage->size().height)
            my = backgroundImage->size().height - resizedMask->size().height - 5;
        
        else
            my = my_prime;
    }
}

Mat CompositeCanvas::loadImage(string imagePath)
{
    Mat img = imread(imagePath, CV_LOAD_IMAGE_UNCHANGED);
    Mat tgt;
    cvtColor(img, tgt, CV_BGR2BGRA);
    return tgt;
}

CompositeCanvas::CompositeCanvas()
{
    this->height = 0;
    this->width = 0;
    this->backgroundImage = nullptr;
    this->originalMaskImage = nullptr;
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
   Mat sizedBackground;
   resize(backgrnd, sizedBackground, Size(width, height), 0.0, 0.0, INTER_LINEAR);
   backgroundImage = unique_ptr<Mat>(new Mat(sizedBackground));
}

void CompositeCanvas::setComposite(const string& maskImgPath, const string& originalImagePath)
{
    if(maskImgPath.length() > 0)
    {
        originalMaskImage = unique_ptr<Mat>(new Mat(loadImage(maskImgPath)));
        resizedMask = make_unique<Mat>();
        auto size = originalMaskImage->size();
        maskHeight = size.height;
        maskWidth = size.width;
        border = ImageBorder(maskWidth, maskHeight, Point(0, 0));
    }
        
    if(originalImagePath.length() > 0)
    {
        originalImage = unique_ptr<Mat>(new Mat(loadImage(originalImagePath)));
        resizedOriginal = make_unique<Mat>();
    }
    
    setSupportingStructuresForComposites();
    initPlacement();
}

void CompositeCanvas::alignMaskSize()
{
    resize(*originalMaskImage, *resizedMask, Size(maskWidth, maskHeight), 0.0, 0.0, INTER_LINEAR);
    resize(*originalImage, *resizedOriginal, Size(maskWidth, maskHeight), 0.0, 0.0, INTER_LINEAR);
}

void CompositeCanvas::setSupportingStructuresForComposites()
{
    if(mask_and_original_available())
    {
        variableMap.clear();
        alignMaskSize();
        auto sourceMatrix = form_matrix(*resizedMask, variableMap);
        solver.compute(sourceMatrix);
    }
}

ObjectType CompositeCanvas::hit(Point p)
{
    if(originalMaskImage == nullptr)
        return ObjectType::None;

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
    if(hitType != ObjectType::None)
    {
        showBoundingRectangle = true;
        objectSelected = hitType;
    }
    else
    {
        showBoundingRectangle = false;
        objectSelected = ObjectType::None;

        if(resizedMask != nullptr)
        {
            setSupportingStructuresForComposites();
        }
    }
}

ObjectType CompositeCanvas::getCurrentlySelected()
{
    return objectSelected;
}

void CompositeCanvas::releaseObject()
{
    objectSelected = ObjectType::None;
}

bool CompositeCanvas::only_background_available()
{
    return backgroundImage != nullptr && 
            (originalMaskImage == nullptr || originalImage == nullptr);
}

bool CompositeCanvas::only_src_available()
{
    return backgroundImage == nullptr && 
            mask_and_original_available();
}

bool CompositeCanvas::src_and_background_available()
{
    return backgroundImage != nullptr && mask_and_original_available();
}
           

bool CompositeCanvas::mask_and_original_available()
{
    return  originalMaskImage!= nullptr && originalImage != nullptr;
}

void CompositeCanvas::draw_adornments(Mat canvas)
{
    if(showBoundingRectangle)
    {
        auto placedBorder = translate_to_canvas_coordindates(border);
        placedBorder.draw(canvas);
    }
}

void CompositeCanvas::initPlacement()
{

    if(src_and_background_available())
    {
        int src_cy = maskHeight/2;
        int src_cx = maskWidth/2;

        mx = backgroundImage->size().width/2 - src_cx;
        my = backgroundImage->size().height/2 - src_cy;
    }
    else if(only_src_available())
    {
        int src_cy = maskHeight/2;
        int src_cx = maskWidth/2;

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
        img = *resizedMask;
    }

    else if(resizedOriginal != nullptr && resizedMask != nullptr &&
         resizedOriginal->size().height < resizedMask->size().height &&
         resizedOriginal->size().width < resizedMask->size().width)
    {
        if(backgroundImage != nullptr)
            img = *backgroundImage;
    }

    else if(src_and_background_available())
    {

        Mat background = *backgroundImage;
        Mat resizedMask = *this->resizedMask;;
        Mat resizedOriginal = *this->resizedOriginal;

        int tgt_height = background.size().height;
        int tgt_width = background.size().width;

        if(tgt_width < resizedMask.size().width || tgt_height < resizedMask.size().height)
        {
            throw BackgroundResizedException(resizedOriginal.size().height,
                                            resizedOriginal.size().width,
                                            background.size().width,
                                            background.size().height,
                                            resizedMask.size().width,
                                            resizedMask.size().height);
        }

        if(showBoundingRectangle)
        {
            img = naive_composite(resizedMask, resizedOriginal, background, mx, my);
        }
        else
        {
            img = composite(resizedMask, resizedOriginal, background, solver, variableMap, mx, my);
        }
       
    }

    auto canvas = fill_in_canvas(img, width, height);
    draw_adornments(canvas);
    return canvas;
 }
