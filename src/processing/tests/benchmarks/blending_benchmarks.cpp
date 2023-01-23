#include <benchmark/benchmark.h>
#include "processing.h"
#include <iostream>
#include <string>
#include <opencv2/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <random>
#include <map>
#include <tuple>

using namespace processing;
using namespace std;

static bool is_mask_pixel(Mat& m, unsigned int x, unsigned int y)
{   
    auto size = m.size();
    if(x <= 0 || y <= 0) return false;

    if(x >= size.width - 1 || y >= size.height - 1) return false;

    return m.at<Vec4b>(cv::Point(x, y))[3] > 0;
}

Mat loadBackgroundImage(string path)
{
  auto backgroundImageRGB = imread(path, CV_LOAD_IMAGE_UNCHANGED);

  Mat backgroundImage;
  cvtColor(backgroundImageRGB, backgroundImage, CV_BGR2BGRA);
  return backgroundImage;
}

static void BM_CompositeCalculations(benchmark::State& state) {

    auto backgroundImagePath = "./src/processing/tests/target_images/beach_large.png";
    auto backgroundImage = loadBackgroundImage(backgroundImagePath);

    auto mask = "./src/processing/tests/masks/eagle_mask_large.png";
    auto original = "./src/processing/tests/original_source_images/eagle_large.png";

    auto canvas = CompositeCanvas();
    canvas.setSize(2400, 1600);
    canvas.setBackground(backgroundImage);
    canvas.setComposite(mask, original);

    Mat result;

    for (auto _ : state) {
        // This code gets timed
        result = canvas.currentImg();
    }

    result = canvas.currentImg();
    string fileName =  "large_eagle_results.png";
    imwrite( "./src/processing/tests/benchmark_results/" + fileName, result);

}

static void BM_IsMaskSpeed(benchmark::State& state)
{
  auto mask = "./src/processing/tests/masks/eagle_mask_large.png";
  auto img = loadBackgroundImage(mask);

  random_device rd; // obtain a random number from hardware
  mt19937 gen(rd()); // seed the generator
  uniform_int_distribution<> distrX(0, img.size().width - 1); // define the range
  uniform_int_distribution<> distrY(0, img.size().height - 1); // define the range


  for(auto _ : state)
  {
   
    auto isMask = is_mask_pixel(img, distrX(gen), distrY(gen));
    
  }

}

static void BM_MapSpeed(benchmark::State& state)
{
  auto mask = "./src/processing/tests/masks/eagle_mask_large.png";
  auto img = loadBackgroundImage(mask);

  random_device rd; // obtain a random number from hardware
  mt19937 gen(rd()); // seed the generator
  uniform_int_distribution<> distrX(0, img.size().width - 1); // define the range
  uniform_int_distribution<> distrY(0, img.size().width - 1); // define the range

  map<tuple<int, int>, bool> m;

  for(int x = 0; x < img.size().width; x++)
  {
    for(int y = 0; y < img.size().height; y++)
    {
      auto isMask = is_mask_pixel(img, distrX(gen), distrY(gen));
      m[make_tuple(x, y)] = isMask;
    }
  }

  for(auto _ : state)
  {
   
    auto isMask =m[make_tuple(distrX(gen), distrY(gen))];
    
  }

}

//BENCHMARK(BM_MapSpeed);
//BENCHMARK(BM_IsMaskSpeed);
// Register the function as a benchmark
BENCHMARK(BM_CompositeCalculations);


BENCHMARK_MAIN();