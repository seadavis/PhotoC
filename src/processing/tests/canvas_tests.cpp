#include <gtest/gtest.h>
#include "canvasmanager.h"
#include <iostream>
#include <opencv2/core.hpp>
#include <string_view>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>

using namespace std;
using namespace processing;

class Composites :
    public testing::TestWithParam<tuple<string, string, int, int>> {
};

class CompositeMissingOneImage :
  public testing::TestWithParam<tuple<string, string, string, int, int, string>> {
};

class BoundingRectangleHitData :
  public testing::TestWithParam<tuple<Point, bool, bool, ObjectType>> {
};

class TranslationData :
  public testing::TestWithParam<tuple<string, string, int, int, Point, int, int>>{
};

class BoundingRectangleHitDataConsecutivePoints :
  public testing::TestWithParam<tuple<Point, Point>> {
};

class ScalingImage :
  public testing::TestWithParam<tuple<string, string, Point, Point, Point, Point, string>> {
};

class TestRenderer : public IRenderImages{

  public:
    TestRenderer() {};
    void RenderImage(Mat& m) override;

    Mat outputImage;

};

class TestTransformer : public TransformImage
{
  public:
    TestTransformer(Point p) :TransformImage(p, 0, 0) {};
    ObjectType hitType;
    void OnHit(ObjectType type) override;
};

void TestTransformer::OnHit(ObjectType type)
{
  hitType = type;
}

void TestRenderer::RenderImage(Mat& m)
{
  m.copyTo(outputImage);
}

constexpr string_view ProcessingDirectory = "./src/processing/tests/";

Mat loadBackgroundImage(string path);



INSTANTIATE_TEST_SUITE_P(CompositeCanvas,
                          ScalingImage,
                          testing::Values(
                              make_tuple("penguin", "library", Point(408, 312), Point(0,0), Point(345, 191), Point(15, 15), "penguin_library_no_translation.png"),
                              make_tuple("penguin", "library", Point(408, 312), Point(20,5), Point(365, 196), Point(15, 15), "penguin_library_with_translation.png"),
                              make_tuple("penguin", "library", Point(408, 312), Point(0,0), Point(345, 191), Point(500, 15), "penguin_library_too_big.png"),
                               make_tuple("penguin", "library", Point(408, 312), Point(0,0), Point(345, 191), Point(-5, -170), "penguin_library_too_small.png"),
                              make_tuple("watereagle", "library", Point(425, 252), Point(0,0), Point(474, 301), Point(60, 60), "watereagle_library_no_translation.png"),
                              make_tuple("watereagle", "library", Point(425, 252), Point(0,0), Point(474, 301), Point(60, 600), "watereagle_library_too_big.png"),
                              make_tuple("watereagle", "library", Point(425, 252), Point(0,0), Point(474, 301), Point(-110, -5), "watereagle_library_too_small.png")
                          ) );

INSTANTIATE_TEST_SUITE_P(CompositeCanvas,
                          TranslationData,
                          testing::Values(
                              make_tuple("penguin", "beach", 894, 596, Point(450, 300), 0, 150),
                              make_tuple("penguin", "beach", 894, 596, Point(450, 300), 150, 0),
                              make_tuple("penguin", "beach", 894, 596, Point(450, 300), 150, 150),
                              make_tuple("penguin", "beach", 894, 596, Point(450, 300), 0, 0),
                              make_tuple("moon", "lake", 901, 676, Point(450, 300), -75, 75),
                              make_tuple("balloon", "gothenburg", 901, 676, Point(450, 300), 200, -125),
                              make_tuple("balloon", "gothenburg", 901, 676, Point(450, 300), -525, -415),
                              make_tuple("balloon", "gothenburg", 901, 676, Point(450, 300), 525, 415)
                          ) );

INSTANTIATE_TEST_SUITE_P(CompositeCanvas,
                          BoundingRectangleHitDataConsecutivePoints,
                          testing::Values(
                              make_tuple(Point(637, 592), Point(0,0)),
                              make_tuple(Point(688, 672), Point(690, 674))
                          ) );

INSTANTIATE_TEST_SUITE_P(CompositeCanvas,
                          BoundingRectangleHitData,
                          testing::Values(
                               make_tuple(Point(637, 592), true, true, ObjectType::Image), // displays
                               make_tuple(Point(688, 672), true, true, ObjectType::Image), // displays
                               make_tuple(Point(643, 597), true, true, ObjectType::Image), // displays
                                make_tuple(Point(580, 569),true, true, ObjectType::None), // misses
                                make_tuple(Point(55, 459),true, true, ObjectType::None), // misses
                                make_tuple(Point(0, 0),true, true, ObjectType::None), // misses
                                make_tuple(Point(120, 120),true, true, ObjectType::None), // misses
                                make_tuple(Point(721, 729),true, true, ObjectType::None), //misses
                                make_tuple(Point(637, 592), false, true, ObjectType::Image), // displays
                                make_tuple(Point(688, 672), true, false, ObjectType::None), // background
                                 make_tuple(Point(643, 597), false, false, ObjectType::None) // nothing
                          ) );

INSTANTIATE_TEST_SUITE_P(CompositeCanvas,
                          CompositeMissingOneImage,
                          testing::Values(
                                make_tuple("./src/processing/tests/target_images/library.png","./src/processing/tests/masks/balloon.png", "", 1050, 950, "mask_empty.png"),
                                make_tuple("./src/processing/tests/target_images/library.png","", "./src/processing/tests/original_source_images/balloon.png", 450, 300, "original_empty.png"),
                                make_tuple("./src/processing/tests/target_images/library.png","", "", 1150, 1150, "mask_and_original_empty.png"),
                                make_tuple("./src/processing/tests/target_images/library.png","./src/processing/tests/masks/penguin.png", "./src/processing/tests/original_source_images/balloon.png", 1300, 1300, "mismatch.png")
                          ) );


INSTANTIATE_TEST_SUITE_P(CompositeCanvas,
                         Composites,
                         testing::Values(

                          make_tuple("eagle", "lake", 1300, 1300), 
                          make_tuple("eagle", "beach",1300, 1300),
                          make_tuple("eagle", "beach",1446, 880),
                          make_tuple("eagle", "gothenburg",1300, 1300), 
                          make_tuple("eagle", "liberty",1300, 1300),
                          make_tuple("eagle", "library",1300, 1300),

                          make_tuple("balloon", "lake",1300, 1300), 
                          make_tuple("balloon", "beach",1300, 1300),
                          make_tuple("balloon", "gothenburg",1300, 1300), 
                          make_tuple("balloon", "liberty",1300, 1300),

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
                          make_tuple("penguin", "gothenburg",800, 800), 
                          make_tuple("penguin", "liberty",1300, 1300),
                          make_tuple("penguin", "library",1300, 1300),

                          make_tuple("watereagle", "lake",1300, 1300), 
                          make_tuple("watereagle", "beach",1300, 1300),
                          make_tuple("watereagle", "gothenburg",1300, 1300), 
                          make_tuple("watereagle", "liberty",1300, 1300),
                          make_tuple("watereagle", "library",1300, 1300)
                          
                        ));



TEST(CompositeCanvas, IllegalComposite){

  Mat backgroundImage = loadBackgroundImage("./src/processing/tests/target_images/library.png");

  auto mask = "./src/processing/tests/masks/balloon.png";
  auto original = "./src/processing/tests/original_source_images/balloon.png";

  auto canvas = CompositeCanvas();
  canvas.setSize(150, 150);
  canvas.setBackground(backgroundImage);
  canvas.setComposite(mask, original);

  try
  {
    Mat result = canvas.currentImg();
    FAIL();
  }
  catch(BackgroundResizedException ex)
  {
    SUCCEED();
  }
  catch(exception exception)
  {
    FAIL();
  }

}


TEST(CompositeCanvas, OnlyCompositesSet)
{
    auto backgroundImage = loadBackgroundImage("./src/processing/tests/target_images/liberty.png");
    auto canvas = CompositeCanvas();
    canvas.setSize(800, 450);
    canvas.setComposite("./src/processing/tests/masks/moon.png", "./src/processing/tests/original_source_images/moon.png");

    Mat result = canvas.currentImg();

    imwrite("./src/processing/tests/target_missing_images/only_composites_set.png", result);
}

TEST(CompositeCanvas, OnlyBackgroundSet)
{
    auto backgroundImage = loadBackgroundImage("./src/processing/tests/target_images/liberty.png");
    auto canvas = CompositeCanvas();
    canvas.setSize(800, 450);
    canvas.setBackground(backgroundImage);

    Mat result = canvas.currentImg();

    imwrite("./src/processing/tests/target_missing_images/only_background_set.png", result);
}


TEST_P(BoundingRectangleHitDataConsecutivePoints, MultiStepTests)
{
    auto args = GetParam();
    Point p1 = get<0>(args);
    Point p2 = get<1>(args);
    auto backgroundImage = loadBackgroundImage("./src/processing/tests/target_images/gothenburg.png");
    auto maskPath = "./src/processing/tests/masks/kitten.png";
    auto originalPath = "./src/processing/tests/original_source_images/kitten.png";
  
    auto canvas = CompositeCanvas();
    auto renderer = TestRenderer();
    auto canvasManager = CanvasManager(&canvas, &renderer);

    canvas.setSize(1300, 1300);    
    canvas.setBackground(backgroundImage);
    canvas.setComposite(maskPath, originalPath);

    canvasManager.QueueOperation(make_shared<TapImage>(p1));
    canvasManager.QueueOperation(make_shared<TapImage>(p2));

    Mat result = renderer.outputImage;
    auto outImage = "double_tap_" + to_string(p1.x) + "_" + to_string(p1.y) + ".png";
    imwrite("./src/processing/tests/test_hit_data/" + outImage, result);
    Mat expectedImg = imread("./src/processing/tests/target_hit_data/" + outImage, CV_LOAD_IMAGE_UNCHANGED);
    bool const equal = std::equal(result.begin<uchar>(), result.end<uchar>(), expectedImg.begin<uchar>());
    ASSERT_TRUE(equal);
}

TEST_P(BoundingRectangleHitData, SingleStepTests)
{
    auto args = GetParam();
    auto p = get<0>(args);
    auto backgroundImage = loadBackgroundImage("./src/processing/tests/target_images/gothenburg.png");
    auto maskPath = "./src/processing/tests/masks/kitten.png";
    auto originalPath = "./src/processing/tests/original_source_images/kitten.png";
       

    auto canvas = CompositeCanvas();
    canvas.setSize(1300, 1300);

    bool set_back = get<1>(args);
    bool set_comp = get<2>(args);

    if(set_back)
      canvas.setBackground(backgroundImage);

    if(set_comp)
      canvas.setComposite(maskPath, originalPath);
  
    auto hit = TestTransformer(p);
    auto tap = TapImage(p);
    auto testRenderer = TestRenderer();
    auto canvasManager = CanvasManager(&canvas, &testRenderer);
    canvasManager.QueueOperation(make_shared<TestTransformer>(p));
    canvasManager.QueueOperation(make_shared<TapImage>(p));

    Mat result = testRenderer.outputImage;
    ASSERT_EQ(hit.hitType, get<3>(args));

    auto outImage = "single_tap_" + to_string(p.x) + "_" + to_string(p.y) + "_" + to_string(set_back) + "_" + to_string(set_comp) + ".png";
    imwrite("./src/processing/tests/test_hit_data/" + outImage, result);
    Mat expectedImg = imread("./src/processing/tests/target_hit_data/" + outImage, CV_LOAD_IMAGE_UNCHANGED);
    bool const equal = std::equal(result.begin<uchar>(), result.end<uchar>(), expectedImg.begin<uchar>());
    ASSERT_TRUE(equal);
}

TEST_P(CompositeMissingOneImage, MissingOrMismatching)
{
    auto args = GetParam();
    auto backgroundImage = loadBackgroundImage(get<0>(args));
    auto maskPath = get<1>(args);
    auto originalPath = get<2>(args);
  
    auto canvas = CompositeCanvas();
    canvas.setSize(get<3>(args), get<4>(args));
    canvas.setBackground(backgroundImage);
    canvas.setComposite(maskPath, originalPath);

    Mat result = canvas.currentImg();

    imwrite("./src/processing/tests/test_missing_images/" + get<5>(args), result);
    Mat expectedImg = imread("./src/processing/tests/target_missing_images/" + get<5>(args), CV_LOAD_IMAGE_UNCHANGED);
    bool const equal = std::equal(result.begin<uchar>(), result.end<uchar>(), expectedImg.begin<uchar>());
    ASSERT_TRUE(equal);
}


TEST_P(TranslationData, ValidTranslations) {

  auto args = GetParam();
  auto backgroundImagePath = "./src/processing/tests/target_images/" + get<1>(args) + ".png";
  auto backgroundImage = loadBackgroundImage(backgroundImagePath);

  auto mask = "./src/processing/tests/masks/" + get<0>(args) + ".png";
  auto original = "./src/processing/tests/original_source_images/" + get<0>(args) + ".png";

  auto canvas = CompositeCanvas();
  canvas.setSize(get<2>(args), get<3>(args));
  canvas.setBackground(backgroundImage);
  canvas.setComposite(mask, original);
  canvas.tap(get<4>(args));
  canvas.cursorMoved(get<5>(args), get<6>(args));

  Mat result = canvas.currentImg();

  auto fileName =  get<0>(args) + "_" + get<1>(args) + "_" + to_string(get<5>(args)) + "_" + to_string(get<6>(args)) + ".png";

  imwrite( "./src/processing/tests/test_translations/" + fileName, result);
  Mat expectedMat = imread("./src/processing/tests/target_translations/" + fileName);
  bool const equal = std::equal(result.begin<uchar>(), result.end<uchar>(), expectedMat.begin<uchar>());
  ASSERT_TRUE(equal);
}

TEST_P(ScalingImage, ScaleTests){
  
  auto args = GetParam();
  auto backgroundImagePath = "./src/processing/tests/target_images/" + get<1>(args) + ".png";
  auto backgroundImage = loadBackgroundImage(backgroundImagePath);

  auto mask = "./src/processing/tests/masks/" + get<0>(args) + ".png";
  auto original = "./src/processing/tests/original_source_images/" + get<0>(args) + ".png";

  auto canvas = CompositeCanvas();
  canvas.setSize(800, 547);
  canvas.setBackground(backgroundImage);
  canvas.setComposite(mask, original);

  auto testRenderer = TestRenderer();
  auto canvasManager = CanvasManager(&canvas, &testRenderer);
  
  Point tapPoint_1 = get<2>(args);
  Point movePoint_1 = get<3>(args);
  Point tapPoint = get<4>(args);
  Point movePoint = get<5>(args);

  canvasManager.QueueOperation(make_shared<TapImage>(tapPoint_1));
  canvasManager.QueueOperation(make_shared<TransformImage>(tapPoint_1, movePoint_1.x, movePoint_1.y));
  canvasManager.QueueOperation(make_shared<TapImage>(tapPoint));
  canvasManager.QueueOperation(make_shared<TransformImage>(tapPoint, movePoint.x, movePoint.y));
  canvasManager.QueueOperation(make_shared<TapImage>(Point(0, 0)));

  Mat result = testRenderer.outputImage;

  auto fileName =  get<6>(args);

  imwrite( "./src/processing/tests/test_scaling/" + fileName, result);
  Mat expectedMat = imread("./src/processing/tests/target_scaling/" + fileName);
  bool const equal = std::equal(result.begin<uchar>(), result.end<uchar>(), expectedMat.begin<uchar>());
  ASSERT_TRUE(equal);
}

TEST_P(Composites, BasicComposite) {

  auto args = GetParam();
  auto backgroundImagePath = "./src/processing/tests/target_images/" + get<1>(args) + ".png";
  auto backgroundImage = loadBackgroundImage(backgroundImagePath);

  auto mask = "./src/processing/tests/masks/" + get<0>(args) + ".png";
  auto original = "./src/processing/tests/original_source_images/" + get<0>(args) + ".png";
  auto targetFileName =  get<0>(args) + "_" + get<1>(args) + "_" + to_string(get<2>(args)) + "_" + to_string(get<3>(args)) + ".png";

  auto canvas = CompositeCanvas();
  auto renderer = TestRenderer();
  auto canvasManager = CanvasManager(&canvas, &renderer);

  canvasManager.QueueOperation(make_shared<Resize>(get<2>(args), get<3>(args)));
  canvasManager.QueueOperation(make_shared<BackgroundImageUpdate>(backgroundImage));
  canvasManager.QueueOperation(make_shared<CompositeImageUpdate>(original, mask));
  
  Mat result = renderer.outputImage;

  imwrite( "./src/processing/tests/test_composites/" + targetFileName, result);
  Mat expectedMat = imread("./src/processing/tests/target_composites/" + targetFileName);
  bool const equal = std::equal(result.begin<uchar>(), result.end<uchar>(), expectedMat.begin<uchar>());
  ASSERT_TRUE(equal);
}

Mat loadBackgroundImage(string path)
{
  auto backgroundImageRGB = imread(path, CV_LOAD_IMAGE_UNCHANGED);

  Mat backgroundImage;
  cvtColor(backgroundImageRGB, backgroundImage, CV_BGR2BGRA);
  return backgroundImage;
}