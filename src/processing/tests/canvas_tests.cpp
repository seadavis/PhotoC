#include <gtest/gtest.h>
#include "processing.h"
#include <iostream>
#include <opencv2/core.hpp>

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

Mat loadBackgroundImage(string path);


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
    canvas.setSize(1300, 1300);
    canvas.setBackground(backgroundImage);
    canvas.setComposite(maskPath, originalPath);
  
    canvas.currentImg();
    canvas.tap(p1);
    canvas.tap(p2);
    Mat result = canvas.currentImg();

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
  
    auto hitType = canvas.hit(p);
    ASSERT_EQ(hitType, get<3>(args));
    canvas.tap(p);
    Mat result = canvas.currentImg();

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

TEST_P(Composites, BasicComposite) {

  auto args = GetParam();
  auto backgroundImagePath = "./src/processing/tests/target_images/" + get<1>(args) + ".png";
  auto backgroundImage = loadBackgroundImage(backgroundImagePath);

  auto mask = "./src/processing/tests/masks/" + get<0>(args) + ".png";
  auto original = "./src/processing/tests/original_source_images/" + get<0>(args) + ".png";

  auto canvas = CompositeCanvas();
  canvas.setSize(get<2>(args), get<3>(args));
  canvas.setBackground(backgroundImage);
  canvas.setComposite(mask, original);

  Mat result = canvas.currentImg();

  auto fileName =  get<0>(args) + "_" + get<1>(args) + "_" + to_string(get<2>(args)) + "_" + to_string(get<3>(args)) + ".png";

  imwrite( "./src/processing/tests/test_composites/" + fileName, result);
  Mat expectedMat = imread("./src/processing/tests/target_composites/" + fileName);

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