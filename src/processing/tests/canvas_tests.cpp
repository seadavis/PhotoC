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

Mat loadBackgroundImage(string path);

INSTANTITATE_TEST_SUITE_P(CompositeCanvas,
                          CompositeMissingOneImage,
                          testing::Values(
                                make_tuple("", nullptr, nullptr, 800, 750, "both_composites_nullptr.png"),
                                make_tuple("", "", nullptr, 650, 400, "backkground_empty_mask_empty_original_null.png"),
                                make_tuple(nullptr, "", "", 1300, 1150, "background_null_others_empty.png"),
                                make_tuple("","", "", 1250, 975, "all_empty.png"),
                                make_tuple(nullptr, nullptr, nullptr, 625, 350, "all_null.png"),
                                make_tuple("./src/processing/tests/target_images/library.png","./src/processing/tests/masks/balloon.png", "", 1050, 950, "mask_empty.png"),
                                make_tuple("./src/processing/tests/target_images/library.png","", "./src/processing/tests/original_source_images/balloon.png", 450, 300, "original_empty.png"),
                                make_tuple("./src/processing/tests/target_images/library.png","./src/processing/tests/masks/balloon.png", nullptr, 1050, 950, "original_null.png"),
                                make_tuple("./src/processing/tests/target_images/library.png",nullptr, "./src/processing/tests/original_source_images/balloon.png", 450, 300, "mask_null.png"),
                                make_tuple("./src/processing/tests/target_images/library.png",nullptr, nullptr, 450, 300, "mask_and_original_null.png"),
                                make_tuple("./src/processing/tests/target_images/library.png","", "", 1150, 1150, "mask_and_original_empty.png"),
                                make_tuple("./src/processing/tests/target_images/library.png","./src/processing/tests/masks/balloon.png", "./src/processing/tests/original_source_images/penguin.png", 1050, 950, "mismtach.png")
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


  auto backgroundImagePath = "./src/processing/tests/target_images/library.png";
  auto backgroundImageRGB = imread(backgroundImagePath, CV_LOAD_IMAGE_UNCHANGED);

  Mat backgroundImage;
  cvtColor(backgroundImageRGB, backgroundImage, CV_BGR2BGRA);

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

TEST(CompositeCanvas, BoundingRect)
{
    Mat m = imread("./src/processing/tests/masks/eagle.png", IMREAD_UNCHANGED);

    Mat tgt;
    cvtColor(m, tgt, CV_BGR2BGRA);

    Mat result = processing::bounding_rectangle(tgt);

    imwrite("./test.png", result);

}


TEST(CompositeCanvas, OnlyCompositesSet)
{
    auto backgroundImage = loadBackgroundImage("./src/processing/tests/target_images/liberty.png");
    auto canvas = CompositeCanvas();
    canvas.setSize(800, 450);
    canvas.setComposite("./src/processing/tests/masks/moon.png", "./src/processing/tests/original_source_images/moon.png");

    Mat result = canvas.currentImg();

    imwrite("./src/processing/tests/target_missing_images/only_background_set.png", result);
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

    imwrite("./src/processing/tests/target_missing_images/" + get<5>(args), result);

    /*bool const equal = std::equal(result.begin<uchar>(), result.end<uchar>(), expectedMat.begin<uchar>());
    ASSERT_TRUE(equal);*/
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